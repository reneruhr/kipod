#include "meshmodel_scene.h"
#include "../../render/render_engine.h"
namespace kipod::MeshModels{

/////////////////////////////////////////////////////////////////////////////////////////////////
//PUBLIC INTERFACE              ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

auto MeshModelScene::AddModel(std::filesystem::path path, bool textures) -> MeshModel*
{
    LoadOBJModel(path, textures);
    return static_cast<MeshModel*>(render_objects_.back().get());
}

auto MeshModelScene::AddModel(const MeshModel& model) -> MeshModel*
{
    render_objects_.push_back(
                std::make_unique<MeshModel>(model));
    auto m = static_cast<MeshModel*>(render_objects_.back().get());
    m->SetUniformMaterial();
    opengl_impl_->CreateMeshModelLayout(m);
    NeedsUpdate();
    return m; 
}

auto MeshModelScene::AddModel(std::unique_ptr<MeshModel>&& model) -> MeshModel*
{
    render_objects_.push_back(std::move(model));
    auto m = static_cast<MeshModel*>(render_objects_.back().get());
    m->SetUniformMaterial();
    opengl_impl_->CreateMeshModelLayout(m);
    NeedsUpdate();
    return m; 
}

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
    raytracer_impl_ = std::make_unique<RaytracerScene>(this);

    opengl_impl_->Setup();
    softrenderer_impl_->Setup();
    raytracer_impl_->Setup();

    SetupCoordinateAxis();
    SetupGrid();

    SetupOptions();
    SetupKeys();
    LOG_ENGINE("MeshModel Scene Initialized.");
    LOG_CONSOLE("MeshModel Scene Initialized.");
}

void MeshModelScene::SetupOptions()
{
    Add(kipod::ModeToggle("Normals", false));
    Add(kipod::ModeToggle("Show Cameras", false));
    Add(kipod::ModeToggle("Show Frustum", false));
    Add(kipod::ModeToggle("Colors", true));
    Add(kipod::ModeToggle("Textures", true));
    Add(kipod::ModeToggle("Bounding Box", false));
    Add(kipod::ModeToggle("Wireframe", false));
    Add(kipod::ModeToggle("Emissive",false));
    Add(kipod::ModeToggle("Clipping Mode", true));
    Add(kipod::ModeToggle("Lazy Mode", false));
    Add(kipod::ModeToggle("Show Lights", false));
    Add(kipod::ModeToggle("Prepare Screen", true));
}

void MeshModelScene::SetupKeys()
{
    Add("Left", Key::Left);
    Add("Right", Key::Right);
    Add("Up", Key::Up);
    Add("Down", Key::Down);
    Add("Forward", Key::PageUp);
    Add("Backward", Key::PageDown);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
//DRAWING                          ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

void MeshModelScene::Draw()
{
    if(Toggle("Lazy Mode") && !needs_update_) return;
    if(RenderEngine::ActiveAPI()=="OpenGL"){
        if(Toggle("Prepare Screen")) opengl_impl_->PrepareScreen();
        opengl_impl_->Draw();
    }
    else if(RenderEngine::ActiveAPI()=="SoftRenderer"){
        if(Toggle("Prepare Screen")) softrenderer_impl_->PrepareScreen();
        softrenderer_impl_->Draw();
    }
    else if(RenderEngine::ActiveAPI()=="Raytracer"){
        if(Toggle("Prepare Screen")) raytracer_impl_->PrepareScreen();
        raytracer_impl_->Draw();
    }
    if(Toggle("Prepare Screen")) kipod::RenderManager::Bind(0);

    needs_update_= false;
}

void MeshModelScene::Resize(int w, int h)
{
    RenderScene::Resize(w, h);
    raytracer_impl_->Resize(width_,height_);
}





/////////////////////////////////////////////////////////////////////////////////////////////////
//   MODEL CONTROL                              ///////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////




void MeshModelScene::SetActiveModel(int id)
{
    SetActiveRenderObject(id);
}

MeshModel* MeshModelScene::GetActiveModel()
{
    if(HasModel()) return static_cast<MeshModel*>(GetActiveRenderObject());
    else return nullptr;
}

void MeshModelScene::AddModel(const PrimMeshModel& model)
{
    render_objects_.push_back(
                std::make_unique<PrimMeshModel>(model));
    NeedsUpdate();
}

void MeshModelScene::LoadOBJModel(std::filesystem::path path, bool textured)
{
    MeshModel model = MeshModel(path, textured);
    if(!model.Valid()) return;
    model.SetUniformMaterial();
    opengl_impl_->CreateMeshModelLayout(&model);
    softrenderer_impl_->CreateMeshModelLayout(&model);
    render_objects_.push_back(
                std::make_unique<MeshModel>(std::move(model)));
    NeedsUpdate();
}

void MeshModelScene::LoadPrimitive(Primitive primitive, int numberPolygons)
{
    PrimMeshModel model = PrimMeshModel(primitive, numberPolygons);
    model.SetUniformMaterial();
    opengl_impl_->CreatePrimitiveModelLayout(&model);
    softrenderer_impl_->CreateMeshModelLayout(&model);
    raytracer_impl_->CreateMeshModelLayout(&model);
    render_objects_.push_back(
                std::make_unique<PrimMeshModel>(std::move(model)));
    NeedsUpdate();
}

void MeshModelScene::LoadPrimitive(Quadric quadric)
{
    PrimMeshModel model = PrimMeshModel(quadric);
    model.SetUniformMaterial();
    raytracer_impl_->CreateMeshModelLayout(&model);
    render_objects_.push_back(
                std::make_unique<PrimMeshModel>(std::move(model)));
    NeedsUpdate();
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
            Toggle("Show Cameras").Switch();
        else if(key == Key::X)
            Toggle("Clipping Mode").Switch();
        //TRANSFORM CONTROL:
        else if(key == Key("Left")){
                if(GetActiveModel())
                    GetActiveModel()->world_->Translate({-stepsize,0,0});
                }
        else if(key == Key("Right")){
                if(GetActiveModel())
                    GetActiveModel()->world_->Translate({+stepsize,0,0});
                }
        else if(key == Key("Up")){
                if(GetActiveModel())
                    GetActiveModel()->world_->Translate({0,stepsize,0});
                }
        else if(key == Key("Down")){
                if(GetActiveModel())
                    GetActiveModel()->world_->Translate({0,-stepsize,0});
                }
        else if(key == Key("Forward")){
                if(GetActiveModel())
                    GetActiveModel()->world_->Translate({0,0, stepsize});
                }
        else if(key == Key("Backward")){
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

        NeedsUpdate();
}

void MeshModelScene::ProcessMouseButtons(kipod::MouseButtonEvent &event)
{
    auto button = event.GetButton();

    if(button == MouseButton::Button0 && event.GetEventType() == kipod::EventType::MouseButtonPressed)
        mouse_rotation_active_ = true;
    else if(button == MouseButton::Button0 && event.GetEventType() == kipod::EventType::MouseButtonReleased)
        mouse_rotation_active_ = false;

    NeedsUpdate();
}

void MeshModelScene::ProcessMouseMoves(kipod::MouseMoveEvent &event)
{
    if(mouse_rotation_active_){
        GetActiveCamera()->Rotate(event.x(),event.y());
        NeedsUpdate();
    }
}

void MeshModelScene::Signup()
{
    kipod::Events::Signup(this, kipod::EventCategoryKeyboard); LOG_CATEGORY_NAME(kipod::EventCategoryKeyboard);
    kipod::Events::Signup(this, kipod::EventCategoryMouseButton); LOG_CATEGORY_NAME(kipod::EventCategoryMouseButton);
    kipod::Events::Signup(this, kipod::EventCategoryMouse); LOG_CATEGORY_NAME(kipod::EventCategoryMouse);
}

void MeshModelScene::Receive(std::shared_ptr<kipod::Event> event)
{
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

    std::vector<Vec3> vertices = {vec3(-2,0,0),vec3(10,0,0),
                                  Vec3(0,-2,0),vec3(0,10,0),
                                  Vec3(0,0,-2),vec3(0,0,10),
                                 };
    std::vector<Vec3> colors =   {vec3(1.0,0.5,0.5),vec3(1,0,0),
                                  Vec3(0.5,1.0,0.5),vec3(0,1,0),
                                  Vec3(0.5,0.5,1.0),vec3(0,0,1),
                                  };

    opengl_impl_->CreateCoordinateAxisLayout(vertices, colors);
}

void MeshModelScene::SetupGrid()
{
    grid_ = std::make_unique<kipod::RenderObject>();

    std::vector<Vec3> vertices;

    int n=100;
    for(int i=-n; i<=n; ++i){
        vertices.push_back(Vec3(-n,i,0));
        vertices.push_back(Vec3(n,i,0));
        vertices.push_back(Vec3(i,-n,0));
        vertices.push_back(Vec3(i,n,0));
    }

    opengl_impl_->CreateGridLayout(vertices);
}



}
