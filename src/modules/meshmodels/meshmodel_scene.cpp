#include "meshmodel_scene.h"
#include "../../render/render_engine.h"
namespace kipod::MeshModels{



/////////////////////////////////////////////////////////////////////////////////////////////////
//SETUP                          ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////



void MeshModelScene::Setup()
{
    name_ = "Mesh Model Scene";

    AddCamera(kipod::RenderCamera({0.0f,0.0f,3.0f}));

    AddLight({kipod::LightSource::AMBIENT, vec4(0.0f), vec4(0.1, 0.1, 0.1, 1.0)});
    AddLight({kipod::LightSource::DIFFUSE, vec4(10.0f,5.0f,0.0f,1.0f), vec4(0.2, 0.3, 0.6, 1.0)});
    AddLight({kipod::LightSource::SPECULAR, vec4(5.0f,10.0f,0.0f,1.0f), vec4(1.0f)});
    SetActiveCamera(0);

    opengl_impl_ = std::make_unique<OpenGLScene>(this);
    softrenderer_impl_ = std::make_unique<SoftRendererScene>(this);

    opengl_impl_->Setup();

    SetupCoordinateAxis();
    SetupGrid();

    SetupOptions();
    LOG_ENGINE("MeshModel Scene Initialized.");
    LOG_CONSOLE("MeshModel Scene Initialized.");
}

void MeshModelScene::SetupOptions(){
    Add(kipod::ModeToggle("Normals", false));
    Add(kipod::ModeToggle("Camera Mode", false));
    Add(kipod::ModeToggle("Camera Frustum Mode", false));
    Add(kipod::ModeToggle("Colors", true));
    Add(kipod::ModeToggle("Textures", true));
    Add(kipod::ModeToggle("Bounding Box", false));
    Add(kipod::ModeToggle("Wireframe", false));
    Add(kipod::ModeToggle("Emissive",false));
    Add(kipod::ModeToggle("Clipping Mode", true));
}



/////////////////////////////////////////////////////////////////////////////////////////////////
//DRAWING                          ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


void MeshModelScene::Draw()
{
    if(RenderEngine::ActiveAPI()=="OpenGL")
        opengl_impl_->Draw();
    else if(RenderEngine::ActiveAPI()=="SoftRenderer")
        softrenderer_impl_->Draw();
}





/////////////////////////////////////////////////////////////////////////////////////////////////
//   MODEL CONTROL                              ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////




void MeshModelScene::SetActiveModel(int id){
    SetActiveRenderObject(id);
}

MeshModel* MeshModelScene::GetActiveModel(){
    if(HasModel()) return static_cast<MeshModel*>(GetActiveRenderObject());
    else return nullptr;
}

void MeshModelScene::AddModel(MeshModel && model)
{
    render_objects_.push_back(
                std::make_unique<MeshModel>(
                    std::forward<MeshModel>(model)));
}

void MeshModelScene::LoadOBJModel(std::filesystem::path path, bool textured)
{
    MeshModel *model = new MeshModel(path, textured);
    if(!model->Valid()) return;
    model->SetUniformMaterial();

    std::string name;
    bool foundTexture;
    name = model->tex_ ? (LOG_ENGINE("A Texture was set. Use Tex Shader"), foundTexture=true,  "Textured Triangles")
                       : (LOG_ENGINE("No Texture set. Use Light Shader."), foundTexture=false, "Colored Triangles" );

    model->SetUniformMaterial();
    opengl_impl_->CreateMeshModelLayout(model);
    softrenderer_impl_->CreateMeshModelLayout(model);
    AddModel(std::move(*model));
}

void MeshModelScene::LoadPrimitive(Primitive primitive, int numberPolygons)
{
    PrimMeshModel *model = new PrimMeshModel(primitive, numberPolygons);
    model->SetUniformMaterial();
    opengl_impl_->CreatePrimitiveModelLayout(model);
    AddModel(std::move(*model));
}


//void MeshModelScene::LookAtModel(int camera_id, int model_id){
//    if(NumberOfModels() <= model_id) return;
//    if(NumberOfCameras() <= camera_id) return;
//    kipod::RenderCamera* cam =GetActiveCamera();
//    MeshModel* model = GetActiveModel();
//    cam->UpdateAt(model->Center());
//}



/////////////////////////////////////////////////////////////////////////////////////////////////
//EVENTS                              ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////


void MeshModelScene::ProcessKeys(kipod::KeyPressedEvent &event)
{
        float stepsize = 1.0f;
        auto key = event.GetKeyCode();
        auto mod = event.GetMod();


        //TOGGLES:
        if(key == Key::Space)
            Toggle("Wireframe").Switch();
        else if(key == Key::N)
            Toggle("Normals").Switch();
        else if(key == Key::B)
            Toggle("Bounding Box").Switch();
        else if(key == Key::C)
            Toggle("Camera Mode").Switch();
        else if(key == Key::X)
            Toggle("Clipping Mode").Switch();
        //TRANSFORM CONTROL:
        else if(key == Key::Left){
                if(GetActiveModel())
                    GetActiveModel()->world_->Translate({-stepsize,0,0});
                }
        else if(key == Key::Right){
                if(GetActiveModel())
                    GetActiveModel()->world_->Translate({+stepsize,0,0});
                }
        else if(key == Key::Up){
                if(GetActiveModel())
                    GetActiveModel()->world_->Translate({0,stepsize,0});
                }
        else if(key == Key::Down){
                if(GetActiveModel())
                    GetActiveModel()->world_->Translate({0,-stepsize,0});
                }
        else if(key == Key::PageUp){
                if(GetActiveModel())
                    GetActiveModel()->world_->Translate({0,0, stepsize});
                }
        else if(key == Key::PageDown){
                if(GetActiveModel())
                    GetActiveModel()->world_->Translate({0,0,-stepsize});
                }
        //CAMERA CONTROL:
        else if(key == Key::A && mod == Mod::KIPOD_MOD_CONTROL){
                    GetActiveCamera()->Move({-stepsize,0,0});
                }
        else if(key == Key::D && mod == Mod::KIPOD_MOD_CONTROL){
                    GetActiveCamera()->Move({+stepsize,0,0});
                }
        else if(key == Key::W && mod == Mod::KIPOD_MOD_CONTROL){
                    GetActiveCamera()->Move({0,stepsize,0});
                }
        else if(key == Key::S && mod == Mod::KIPOD_MOD_CONTROL){
                    GetActiveCamera()->Move({0,-stepsize,0});
                }
        else if(key == Key::R && mod == Mod::KIPOD_MOD_CONTROL){
                    GetActiveCamera()->Move({0,0, stepsize});
                }
        else if(key == Key::F && mod == Mod::KIPOD_MOD_CONTROL){
                    GetActiveCamera()->Move({0,0,-stepsize});
                }

        else if(key == Key::A && mod == Mod::None){
                    GetActiveCamera()->Move(kipod::RenderCamera::Movement::LEFT, stepsize);
                }
        else if(key == Key::D && mod == Mod::None){
                    GetActiveCamera()->Move(kipod::RenderCamera::Movement::RIGHT, stepsize);
                }
        else if(key == Key::W && mod == Mod::None){
                    GetActiveCamera()->Move(kipod::RenderCamera::Movement::FORWARD, stepsize);
                }
        else if(key == Key::S && mod == Mod::None){
                    GetActiveCamera()->Move(kipod::RenderCamera::Movement::BACKWARD, stepsize);
                }
        else if(key == Key::R && mod == Mod::None){
                    GetActiveCamera()->Move(kipod::RenderCamera::Movement::UP, stepsize);
                }
        else if(key == Key::F && mod == Mod::None){
                    GetActiveCamera()->Move(kipod::RenderCamera::Movement::DOWN, stepsize);
                }

}

void MeshModelScene::ProcessMouseButtons(kipod::MouseButtonEvent &event)
{
    auto button = event.GetButton();

    if(button == MouseButton::Button0 && event.GetEventType() == kipod::EventType::MouseButtonPressed)
        mouse_rotation_active_ = true;
    else if(button == MouseButton::Button0 && event.GetEventType() == kipod::EventType::MouseButtonReleased)
        mouse_rotation_active_ = false;
}

void MeshModelScene::ProcessMouseMoves(kipod::MouseMoveEvent &event)
{
    if(mouse_rotation_active_)
        GetActiveCamera()->Rotate(event.x(),event.y());
}

void MeshModelScene::Signup() {
    kipod::Events::Signup(this, kipod::EventCategoryKeyboard); LOG_CATEGORY_NAME(kipod::EventCategoryKeyboard);
    kipod::Events::Signup(this, kipod::EventCategoryMouseButton); LOG_CATEGORY_NAME(kipod::EventCategoryMouseButton);
    kipod::Events::Signup(this, kipod::EventCategoryMouse); LOG_CATEGORY_NAME(kipod::EventCategoryMouse);
}

void MeshModelScene::Receive(std::shared_ptr<kipod::Event> event){
    Process<kipod::KeyPressedEvent>(event, BIND_EVENT_FN(MeshModelScene::ProcessKeys));
    Process<kipod::MouseButtonPressEvent>(event, BIND_EVENT_FN(MeshModelScene::ProcessMouseButtons));
    Process<kipod::MouseButtonReleaseEvent>(event, BIND_EVENT_FN(MeshModelScene::ProcessMouseButtons));
    Process<kipod::MouseMoveEvent>(event, BIND_EVENT_FN(MeshModelScene::ProcessMouseMoves));
}




/////////////////////////////////////////////////////////////////////////////////////////////////
//COORDINATE AXIS AND GRID                              ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////




void MeshModelScene::SetupCoordinateAxis()
{
    coordinate_axis_ = std::make_unique<kipod::RenderObject>();

    std::vector<vec3> vertices = {vec3(-2,0,0),vec3(10,0,0),
                                  vec3(0,-2,0),vec3(0,10,0),
                                  vec3(0,0,-2),vec3(0,0,10),
                                 };
    std::vector<vec3> colors =   {vec3(1.0,0.5,0.5),vec3(1,0,0),
                                  vec3(0.5,1.0,0.5),vec3(0,1,0),
                                  vec3(0.5,0.5,1.0),vec3(0,0,1),
                                  };

    opengl_impl_->CreateCoordinateAxisLayout(vertices, colors);
}

void MeshModelScene::SetupGrid()
{
    grid_ = std::make_unique<kipod::RenderObject>();

    std::vector<vec3> vertices;

    int n=100;
    for(int i=-n; i<=n; ++i){
        vertices.push_back(vec3(-n,i,0));
        vertices.push_back(vec3(n,i,0));
        vertices.push_back(vec3(i,-n,0));
        vertices.push_back(vec3(i,n,0));
    }

    opengl_impl_->CreateGridLayout(vertices);
}



}
