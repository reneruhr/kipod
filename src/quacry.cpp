#include "../include/quacry.h"

#include "scene.h"
#include "../include/utils/mat.h"
#include "../include/pointset.h"


#include "../include/gui.h"


QuaCry::QuaCry(Scene* scene, std::vector< float > window_size, std::vector< int > sample_size )
        : PointSet(sample_size),  WindowBox(window_size), GUIModule(), GUIMathControl(), scene_(scene)
{
    Init();
}


QuaCry::QuaCry(Scene *scene) : PointSet(), WindowBox(), GUIModule(), GUIMathControl(), scene_(scene)
{
    Init();
}

void QuaCry::Init()
{
    LOG("Init QuaCry with WindowBox {}, {} ,{}, {} ,{} ,{} ,{}, {}",
        window_size_[0],window_size_[1],window_size_[2],window_size_[3],window_size_[4],window_size_[5],
            window_size_[6],window_size_[7]);

    PointSet::Init(scene_->_glrenderer);
    scene_->pointsets.push_back(this);

    camera_ = new Camera( Left(), Right(),
                          Bottom(), Top(),
                          Near(), Far() );
    //sideViewCamera_ = new Camera( -10, -10, -10, 10, -10, 10);
    sideViewCamera_ = new Camera(45, float(scene_->_width)/scene_->_height, 0.1f, 200.0);
    sideViewCamera_->updateLookAt(vec3(30,0,0),vec3(60,0,0),vec3(0,0,1));

    scene_->addCamera(sideViewCamera_);
    scene_->addCamera(camera_, false);
    scene_->setLastCameraActive();
    //        window_box_ = new PrimMeshModel(Primitive::Cube);
    //        window_size_ =mat4(vec4{10.0,10.0,1.0,1.0});
    //        window_box_->moveLocal(window_size_);
    //        window_box_->setUniformMaterial();
    //        scene->models.push_back(window_box_);
    //        scene->models.back()->init(scene->_glrenderer);

    LOG("Inititalized QuaCry");
}

void QuaCry::Draw()
{
        if (ImGui::CollapsingHeader("Lattice:")){

            static mat4 f_basis = basis_;
            static mat4 f_m = current_transform_;
            ImGui::Columns(2, NULL, true);
            ImGui::Text("Basis:");
            matrix4(f_basis);
            ImGui::NextColumn();
            ImGui::Text("Current Transformation:");
            matrix4(f_m);
            ImGui::Columns(1);
        }
//        if (ImGui::CollapsingHeader("Sample Size")){
//            GUIMathControl.verify4i(sampleSize, currentMode);
//            if(currentMode == NEW_SAMPLE){
//                lattice.new_sample(sampleSize);
//                glClearColor(0, 0, 0, 0);
//                GLfloat vertices_position[lattice.sample_num*4];

//                lattice.set_vertices_position(vertices_position, 1);
//                render.initialize(vao, vertices_position, lattice.sample_num);

//                currentMode = IDLE;
//            }
//        }


        static std::array<int, 16> selectedEmbedding { 1, 1, 0, 0,
                                                       1, 1, 0, 0,
                                                       0, 0, 0, 0,
                                                       0, 0, 0, 0 };

        if (ImGui::CollapsingHeader("Embedded SL2:"))
        {
            ImGui::Columns(3, NULL, true);
            embeddings(selectedEmbedding, currEmbedding);
            ImGui::NextColumn();
            embeddingsView(selectedEmbedding);
            ImGui::Columns(1);
            sl2control(temporaryMatrixView, SL4walk, currEmbedding);
        }
        if (ImGui::CollapsingHeader("Walk")){
           sl4control(SL4walk);
        }


//        if (ImGui::CollapsingHeader("Window Options")){
//            ImGui::Text("Size");
//            ImGui::SliderFloat4("zw window", windowBox.data(), -10.0f, 10.0f);
//        }

//        if (ImGui::CollapsingHeader("Scale Options")){
//            ImGui::SliderFloat("scale", &scale, 0.1, 20.0f);
//        }

        world_transform_ = temporaryMatrixView;
}
