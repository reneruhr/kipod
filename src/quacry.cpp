#include "../include/quacry.h"

#include "scene.h"
#include "../include/utils/mat.h"
#include "../include/pointset.h"


#include "../include/gui.h"


QuaCry::QuaCry(Scene* scene, mat4 basis,
               std::vector< float > window_size,
               std::vector< int > sample_size,
               WindowType type,
               Shape shape)
        : PointSet(sample_size, basis),
          WindowBox(window_size),
          GUIModule(),
          GUIMathControl(),
          Shape(shape),
          scene_(scene),
          window_type_(type)
{
    Init();
}


QuaCry::QuaCry(Scene *scene) : PointSet(), WindowBox(), GUIModule(), GUIMathControl(), Shape(Square()), scene_(scene)
{
    Init();
}

void QuaCry::Init()
{
    PointSet::Init(scene_->_glrenderer, window_type_);
    scene_->AddPointSet(this);

    camera_ = new Camera( Left(), Right(),
                          Bottom(), Top(),
                          Near(), Far() );
    //sideViewCamera_ = new Camera( -10, -10, -10, 10, -10, 10);
    sideViewCamera_ = new Camera(45, float(scene_->_width)/scene_->_height, 0.1f, 200.0);
    sideViewCamera_->updateLookAt(vec3(30,0,0),vec3(60,0,0),vec3(0,0,1));

    scene_->addCamera(sideViewCamera_);
    scene_->addCamera(camera_, false);
    scene_->setLastCameraActive();

    Shape::Init(scene_->_glrenderer);
    scene_->AddShape(this);

    LOG("Inititalized QuaCry");
}

void QuaCry::Draw()
{
    if (ImGui::CollapsingHeader("Quasicrystals")){


    if (ImGui::TreeNode("Lattice:")){
            static mat4 f_basis;
            f_basis = basis_;
            static mat4 f_m;
            f_m= PointSet::world_transform_;
            ImGui::Columns(2, NULL, true);
            ImGui::Text("Basis:");
            matrix4(f_basis);
            ImGui::NextColumn();
            ImGui::Text("Current Transformation:");
            matrix4(f_m);
            ImGui::Columns(1);
            ImGui::TreePop();
    }
        static std::array<int, 16> selectedEmbedding { 1, 1, 0, 0,
                                                       1, 1, 0, 0,
                                                       0, 0, 0, 0,
                                                       0, 0, 0, 0 };
        if (ImGui::TreeNode("Embedded SL2:")){
            ImGui::Columns(3, NULL, true);
            embeddings(selectedEmbedding, currEmbedding);
            ImGui::NextColumn();
            embeddingsView(selectedEmbedding);
            ImGui::Columns(1);
            sl2control(temporaryMatrixView, SL4walk, currEmbedding);
            ImGui::TreePop();
        }
        PointSet::world_transform_ = temporaryMatrixView;

        if (ImGui::TreeNode("Walk")){
           sl4control(SL4walk);
           ImGui::TreePop();
        }



        if (ImGui::TreeNode("View:")){
            static int selected_view = -1;
            for (int n = 0; n <  scene_->numberOfCameras(); n++){
                char buf[64];
                if(n==0) sprintf(buf, "Perspective Projection to XYZ (close up) ");
                else if(n==1) sprintf(buf, "Perspective Projection to XYZ (far away)");
                else if(n==2) sprintf(buf, "Cut and Project");

                if (ImGui::Selectable(buf, selected_view == n)){
                    selected_view = n;
                    scene_->setActiveCamera(selected_view);
                }
            }
            ImGui::TreePop();
        }

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


//        if (ImGui::CollapsingHeader("Window Options")){
//            ImGui::Text("Size");
//            ImGui::SliderFloat4("zw window", windowBox.data(), -10.0f, 10.0f);
//        }

//        if (ImGui::CollapsingHeader("Scale Options")){
//            ImGui::SliderFloat("scale", &scale, 0.1, 20.0f);
//        }


}
