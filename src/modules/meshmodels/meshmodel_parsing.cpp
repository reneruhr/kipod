#include "meshmodel_parsing.h"
#include "../../utils/obj_parsing.h"
#include "../../utils/buffer_packing.h"
namespace kipod::MeshModels{

void LoadFile(std::filesystem::path path,
              std::shared_ptr<std::vector<Vec3> > vertices_vector, std::shared_ptr<std::vector<unsigned int> > indices_vector,
              std::shared_ptr<std::vector<Vec3> > normals_vector, std::shared_ptr<std::vector<unsigned int> > nindices_vector,
              std::shared_ptr<std::vector<Vec2> > texture_vector, std::shared_ptr<std::vector<unsigned int> > tindices_vector)
              {
    LOG_ENGINE("Start loading MeshModel from File.");

    std::ifstream ifile(path);
    std::vector<FaceIdcs> faces;
    bool hasNormals=false;
    bool hasTextures=false;

    while (!ifile.eof())
    {
        std::string curLine;
        getline(ifile, curLine);

        //LOG_ENGINE(curLine);

        std::istringstream issLine(curLine);
        std::string lineType;

        issLine >> std::ws >> lineType;

        if (lineType == "v")
            vertices_vector->push_back(vec3fFromStream(issLine));
        else if (lineType == "vn"){
            hasNormals=true;
            normals_vector->push_back(vec3fFromStream(issLine));}
        else if (lineType == "vt"){
            hasTextures=true;
            texture_vector->push_back(vec2fFromStream(issLine));}
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

    int vs = size(*vertices_vector);
    int ns = size(*normals_vector);
    int ts = size(*texture_vector);

    for (std::vector<FaceIdcs>::iterator it = faces.begin(); it != faces.end(); ++it)
    {
        for (int i = 0; i < 3; i++)
        {
            if(it->v[i] < 0)
                it->v[i] = vs + it->v[i]+1;
            indices_vector->push_back(it->v[i]-1);

            if(hasNormals){
                if(it->vn[i] < 0)
                    it->vn[i] = ns + it->vn[i]+1;
                nindices_vector->push_back(it->vn[i]-1);
            }
            if(hasTextures){
                if(it->vt[i] < 0)
                    it->vt[i] = ts + it->vt[i]+1;
                tindices_vector->push_back(it->vt[i]-1);
            }
        }
    }
}

bool isClose(const float& a, const float& b){
    return std::abs(a-b) <= std::numeric_limits<float>::epsilon();
}

bool isRelClose(const float& a, const float& b){
    return std::abs(a-b) <= std::max(std::abs(a),std::abs(b))
                           *std::numeric_limits<float>::epsilon();
}

bool isRelLess(const float& a, const float& b){
    return a < b + std::numeric_limits<float>::epsilon();
}

bool isClose(const Vec3& v, const Vec3& w){
    return isClose(v.x,w.x) &&
           isClose(v.y,w.y) &&
           isClose(v.z,w.z);
}

bool lessVector(const Vec3& v, const Vec3& w){
    if(isClose(v,w)) return false;
    else return isRelLess(v.x,w.x)
            ||  (isClose(v.x,w.x) && isRelLess(v.y,w.y))
            ||  (isClose(v.x,w.x) && isClose(v.y,w.y) && isRelLess(v.z,w.z));
}

struct compareVector {
    bool operator()(const Vec3& v, const Vec3& w) const {
        return lessVector(v,w);
    }
};

void ReduceVertices(std::vector<Vec3>& vertices_vector, std::vector<unsigned int>& indices_vector)
{
    std::map<Vec3, unsigned int, compareVector> vertices_map;
    std::vector<Vec3> new_vertices;
    for(unsigned int j = 0, l=0; j<indices_vector.size(); ++j){
        unsigned int i = indices_vector[j];
        auto [it,success] = vertices_map.insert(std::pair<Vec3, unsigned int>(
                    vertices_vector[i], l));
        if(success) {
            new_vertices.push_back(vertices_vector[i]);
            indices_vector[j]=l++;
        }
        else indices_vector[j] = it->second;

    }
    vertices_vector = new_vertices;
}

void forgetSize( glm::vec3& v )
{
    float l = glm::length(v);
    float EPSILON_FLOAT = std::numeric_limits<float>::epsilon();
    if(l>EPSILON_FLOAT) v /= l;
}

void CalculateNormals(std::shared_ptr<std::vector<Vec3> > vertices_vector, std::shared_ptr<std::vector<unsigned int> > indices_vector,
                      std::shared_ptr<std::vector<Vec3> > normals_vector, std::shared_ptr<std::vector<unsigned int> > nindices_vector)
{
    LOG_ENGINE("Calculate Normals from Faces");

    std::vector<Vec3> face_normals;
    for(unsigned int k=0; k < indices_vector->size(); k+=3)
        face_normals.emplace_back(
                              normalize(
                              cross(
                                    (*vertices_vector)[(*indices_vector)[k+1]]-(*vertices_vector)[(*indices_vector)[k]],
                                    (*vertices_vector)[(*indices_vector)[k+2]]-(*vertices_vector)[(*indices_vector)[k+1]]
                                  )
                             ));

    nindices_vector->resize(indices_vector->size());

    for(unsigned int i = 0, max = vertices_vector->size(); i<max ; ++i){
        std::vector<Vec3> local_normals;
        std::vector<unsigned int>::iterator it = std::find(indices_vector->begin(), indices_vector->end(), i);
        int k;
        
        while(it!=indices_vector->end()){
            k = std::distance(indices_vector->begin(), it);
            local_normals.push_back(face_normals[k/3]);
            (*nindices_vector)[k]=i;
            it = std::find(++it, indices_vector->end(), i);
        }
        if(local_normals.empty()) continue;
        normals_vector->emplace_back(
                    std::accumulate(local_normals.begin(), local_normals.end(), Vec3(0.0f))
                    );
        forgetSize(normals_vector->back());
    }

}
}
