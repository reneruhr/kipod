#include "core.h"

#include "meshmodel.h"
#include "utils/vec.h"

#include "utils/obj_parsing.h"

#include <numeric>
#include <map>
#include <filesystem>


using namespace std;


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
        tex_ = new kipod::Texture;

        auto end = std::end(fileName);
        *(end-3) = 'p'; *(end-2) = 'n'; *(end-1) = 'g';
        if(std::filesystem::exists(fileName)){
            tex_->LoadTexture(fileName.c_str());
            return;
         }
        *(end-3) = 'j'; *(end-2) = 'p'; *(end-1) = 'g';
        if(std::filesystem::exists(fileName)){
            tex_->LoadTexture(fileName.c_str());
            return;
         }
        *(end-3) = 's'; *(end-2) = 'v'; *(end-1) = 'g';
        if(std::filesystem::exists(fileName)){
            tex_->LoadTexture(fileName.c_str());
            return;
         }
        LOG_INFO("Tried but did not succeed to load texture.");
        delete tex_;
        tex_ = nullptr;
    }
}


MeshModel::~MeshModel(void)
{
}


void MeshModel::loadFile(string fileName)
{
    LOG_ENGINE("Start loading MeshModel from File.");
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

        //LOG_ENGINE(curLine);

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
            LOG_ENGINE("Found unknown line Type \" {} \"", curLine);
		}
	}
    LOG_ENGINE("Finished Parsing");

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


void MeshModel::Init(bool textured, bool normals)
{
    if(textured){
        CreateTriangleVector();
        triangles_indices_ = vector<unsigned int>(triangles_.size()*3);
        std::iota(std::begin(triangles_indices_), std::end(triangles_indices_), 0);
        //    modelTexturedData = glrenderer->LoadGLTriangles(&triangles_, &triangles_indices_);
        auto layout = Layout("Textured Triangles");
        static_cast<kipod::GLRenderLayout*>(layout)->SetupGLTriangles(&triangles_, &triangles_indices_);
    }else if(normals){
        auto layout = static_cast<kipod::GLRenderLayout*>(Layout("Colored Triangles"));
        layout->SetupColoredTriangles(&vertices_vector,&indices_vector,
                                               &normals_vector, &nindices_vector);
    }else{
        auto layout = static_cast<kipod::GLRenderLayout*>(Layout("Colored Triangles"));
        layout->SetupColoredTriangles(&vertices_vector,&indices_vector);
    }


    modelDataWired =make_shared<ModelData>(ModelData());
    modelDataWired->indices_size = size(indices_vector);
    if(!normals_vector.empty()) modelDataWired->hasNormals = false;
}


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



void MeshModel::move(const vec3& translate){
    _world_transform=Translate(translate)*_world_transform;
}

void MeshModel::createBBox(){
    _boundingBoxData = BoundingBoxData(vertices_vector);
}

mat4 MeshModel::getmTransform() const{
    return _world_transform*_local_transform;
}
glm::mat4 MeshModel::getmTransformBBox()  {
    //return _world_transform*_local_transform*_boundingBoxData._transform;
    return Transform() * MakeGLM(_boundingBoxData._transform);
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
    MaterialStruct material =  MaterialStruct();
    colors_vector = { material };
    cindices_vector = vector<unsigned int>((indices_vector).size(), 0);

    mat_ =  new kipod::RenderMaterial;
}
