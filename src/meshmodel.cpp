#include "../include/core.h"

#include "../include/meshmodel.h"
#include "../include/utils/vec.h"

#include <numeric>
#include <map>

using namespace std;

struct FaceIdcs
{
    int v[4];
    int vn[4];
    int vt[4];

	FaceIdcs()
	{
		for (int i=0; i<4; i++)
			v[i] = vn[i] = vt[i] = 0;
	}

	FaceIdcs(std::istream & aStream)
	{
		for (int i=0; i<4; i++)
			v[i] = vn[i] = vt[i] = 0;

		char c;
		for(int i = 0; i < 3; i++)
		{

            aStream >> std::ws >> v[i] >> std::ws;
			if (aStream.peek() != '/')
				continue;
			aStream >> c >> std::ws;
			if (aStream.peek() == '/')
			{
				aStream >> c >> std::ws >> vn[i];
				continue;
			}
			else
				aStream >> vt[i];
			if (aStream.peek() != '/')
				continue;
			aStream >> c >> vn[i];
		}
	}
};

vec3 vec3fFromStream(std::istream & aStream)
{
	float x, y, z;
	aStream >> x >> std::ws >> y >> std::ws >> z;
	return vec3(x, y, z);
}

vector<int> vec3iFromStream(std::istream & aStream)
{
    vector<int> vec;
    aStream >> vec[0] >> std::ws >> vec[1] >> std::ws >> vec[2];
    return vec;
}

vec2 vec2fFromStream(std::istream & aStream)
{
	float x, y;
	aStream >> x >> std::ws >> y;
	return vec2(x, y);
}


BoundingBoxData::BoundingBoxData(const vector<vec3> &vertices){
        vec3 min = vertices[0];
        vec3 max = vertices[0];
        for (auto v : vertices){
            if (v[0] < min[0]) min[0] = v[0];
            if (v[0] > max[0]) max[0] = v[0];
            if (v[1] < min[1]) min[1] = v[1];
            if (v[1] > max[1]) max[1] = v[1];
            if (v[2] < min[2]) min[2] = v[2];
            if (v[2] > max[2]) max[2] = v[2];
        }
        _size = (max-min)/2;
        _center= (max+min)/2;
        _transform = Translate(_center)*Scale(_size);
    }


MeshModel::MeshModel(string fileName, bool textured)
{
    _world_transform = mat4(1.0);
	loadFile(fileName);
	createBBox();
	centerModel();

    if(textured){
        texture = new Texture;
        string str = "obj";
        fileName.replace(fileName.find(str),str.length(),"png"); // File better not has obj in its name!
        texture->LoadTexture(fileName.c_str());
    }
}


MeshModel::~MeshModel(void)
{
}

void MeshModel::loadFile(string fileName)
{
	ifstream ifile(fileName.c_str());
	vector<FaceIdcs> faces;
    bool hasNormals=false;
    bool hasTextures=false;

	// while not end of file
	while (!ifile.eof())
	{
		// get line
		string curLine;
		getline(ifile, curLine);

        LOG_INFO(curLine);

		// read type of the line
		istringstream issLine(curLine);
		string lineType;

		issLine >> std::ws >> lineType;


		// based on the type parse data
		if (lineType == "v") 
			vertices_vector.push_back(vec3fFromStream(issLine));
		else if (lineType == "vn"){
			hasNormals=true;
			normals_vector.push_back(vec3fFromStream(issLine));}
        else if (lineType == "vt"){
            hasTextures=true;
            texture_vector.push_back(vec2fFromStream(issLine));}
        else if (lineType == "f"){
			faces.push_back(issLine);
        }
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
            LOG_INFO("Found unknown line Type \" {} \"", curLine);
		}
	}
	std::cout << " Finished Parsing " << std::endl;
    LOG_INFO("Finished Parsing");

    int vs = size(vertices_vector);
    int ns = size(normals_vector);
    int ts = size(texture_vector);


	for (vector<FaceIdcs>::iterator it = faces.begin(); it != faces.end(); ++it)
	{

        for (int i = 0; i < 3; i++)
		{
            if(it->v[i] < 0)
                it->v[i] = vs + it->v[i]+1;
            indices_vector.push_back(it->v[i]-1);

            if(hasNormals){
                if(it->vn[i] < 0)
                    it->vn[i] = ns + it->vn[i]+1;
                nindices_vector.push_back(it->vn[i]-1);
            }
            if(hasTextures){
                if(it->vt[i] < 0)
                    it->vt[i] = ts + it->vt[i]+1;
                tindices_vector.push_back(it->vt[i]-1);
            }
		}
	}
    if(hasNormals == false) calculateNormals();
}

struct compareVector {
    bool operator()(const vec3& v, const vec3& w) const {
        return lessVector(v,w);
    }
};

void MeshModel::reduceVertices(){
    std::map<vec3, unsigned int, compareVector> vertices_map;
    vector<vec3> new_vertices;
    for(unsigned int j = 0, l=0; j<indices_vector.size(); ++j){
        unsigned int i = indices_vector[j];
        auto [it,success] = vertices_map.insert(std::pair<vec3, unsigned int>(
                    vertices_vector[i], l));
        if(success) {
            new_vertices.push_back(vertices_vector[i]);
            indices_vector[j]=l++;
            LOG_DEBUG("Found a new vector {} with index {}", it->first,it->second);

        }
        else {indices_vector[j] = it->second;
             LOG_DEBUG("Found an old vector {} with index {}", it->first,it->second);
             }
    }
    vertices_vector = new_vertices;
}

void MeshModel::calculateNormals(){
     LOG_INFO("Calculate Normals from Faces");

    vector<vec3> face_normals;
    for(unsigned int k=0; k<indices_vector.size(); k+=3)
    face_normals.emplace_back(
                              normalize(
                              cross(
                                    vertices_vector[indices_vector[k+1]]-vertices_vector[indices_vector[k]],
                                    vertices_vector[indices_vector[k+2]]-vertices_vector[indices_vector[k+1]]
                                  )
                             ));

    nindices_vector.resize(indices_vector.size());
    for(unsigned int i = 0, max = vertices_vector.size(); i<max ; ++i){
        vector<vec3> local_normals;
        std::vector<unsigned int>::iterator it = std::find(indices_vector.begin(), indices_vector.end(), i);
        int k;
        while(it!=indices_vector.end()){
            k = std::distance(indices_vector.begin(), it);
            local_normals.push_back(face_normals[k/3]);
            nindices_vector[k]=i;
            it = std::find(++it, indices_vector.end(), i);
        }
        if(local_normals.empty()) continue;
        normals_vector.emplace_back(
                    std::accumulate(local_normals.begin(), local_normals.end(), vec3(0.0f))
                    );
        forgetSize(normals_vector.back());
    }

}

//void MeshModel::calculateNormals(){
//     LOG_INFO("Calculate Normals from Faces");

//     vector<vec3> face_normals;
//    for(unsigned int k=0; k<indices_vector.size(); k+=3){
//    vec3 w = normalize(
//                        cross(
//                                vertices_vector[indices_vector[k+1]]-vertices_vector[indices_vector[k]],
//                                vertices_vector[indices_vector[k+2]]-vertices_vector[indices_vector[k+1]]
//                              )
//                      );
//    nindices_vector.insert(nindices_vector.end(), {k/3, k/3, k/3});
//    normals_vector.push_back(w);
//    }

//}

void MeshModel::draw(SoftRenderer *softrenderer, bool wireframemode, bool clippingMode, bool normals)
{
    if(normals)
        softrenderer->DrawTriangles(&vertices_vector, &indices_vector,
                                wireframemode, clippingMode,
                                &normals_vector, &nindices_vector);
    else
        softrenderer->DrawTriangles(&vertices_vector, &indices_vector, wireframemode);
}

void MeshModel::drawWithLight(SoftRenderer *softrenderer, const std::vector<Light*> &lights, bool lightMode, bool emissiveMode){
        softrenderer->DrawColoredTriangles(&vertices_vector, &indices_vector,
                                    &normals_vector, &nindices_vector,
                                    &colors_vector, &cindices_vector,
                                    lights,lightMode, emissiveMode);
}


void MeshModel::init(GLRenderer *glrenderer, bool colored)
{
    if(colored)
        modelData =
                 glrenderer->loadColoredTriangles(&vertices_vector,&indices_vector,
                                           &normals_vector, &nindices_vector);

    modelDataWired =
                 glrenderer->loadTriangles(&vertices_vector,&indices_vector,
                                           &normals_vector, &nindices_vector);
}

void MeshModel::Init(GLRenderer *glrenderer)
{
    CreateTriangleVector();
    modelTexturedData = glrenderer->LoadGLTriangles(&triangles_, &indices_vector);
}

void MeshModel::Draw(GLRenderer *glrenderer)
{
    glrenderer->DrawGLTriangles(modelTexturedData);
}

void MeshModel::draw(GLRenderer *glrenderer)
{
    glrenderer->drawTriangles(modelDataWired);
}
void MeshModel::drawNormals(GLRenderer *glrenderer)
{
    glrenderer->drawNormals(modelDataWired);
}

void MeshModel::drawColored(GLRenderer *glrenderer)
{
    glrenderer->drawColoredTriangles(modelData);
}

void MeshModel::move(const vec3& translate){
	_world_transform=Translate(translate)*_world_transform;
}

void MeshModel::createBBox(){
    _boundingBoxData = BoundingBoxData(vertices_vector);
}

mat4 MeshModel::getmTransform(){
    return _world_transform*_local_transform;
}
mat4 MeshModel::getmTransformBBox(){
    return _world_transform*_local_transform*_boundingBoxData._transform;
}

void MeshModel::processEvent(Event &event)
{

}

vec3 MeshModel::getCenter(){
	mat4 m =_world_transform*_local_transform;
	vec3 center = vec3(m[0][3],m[1][3],m[2][3]);
	return center;
}

void MeshModel::centerModel(){
    _local_transform = Translate(-_boundingBoxData._center);
}

void MeshModel::moveLocal(const mat4& transform){
	_local_transform = transform*_local_transform;
}

void MeshModel::moveWorld(const mat4& transform){
    _world_transform = transform*_world_transform;
}

void MeshModel::setLocal(const mat4 &transform)
{
    _local_transform=transform;
}

void MeshModel::setUniformMaterial(MaterialStruct &material){
    colors_vector = { material };
    cindices_vector = vector<unsigned int>((indices_vector).size(), 0);
}

void MeshModel::setUniformMaterial(){
    MaterialStruct material = MaterialStruct();
    colors_vector = { material };
    cindices_vector = vector<unsigned int>((indices_vector).size(), 0);
}
