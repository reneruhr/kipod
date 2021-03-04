#include "../../kipod.h"
#include "../../math/lattice.h"

namespace kipod::QuasiCrystals{
using Vector = vec4;
using Basis = mat4;
using Sample = std::vector< int >;

struct LatticeData{
    GLuint vao, vbo;
    GLuint u_binding_point= 1, u_buffer, u_block_index;
    GLuint u_binding_point_window= 2, u_buffer_window, u_block_index_window;
    int size;
    float point_size_=5.0f;
    float point_size_window_ = 3.0f;
    float alpha_ = 0.001f;
    float z_decay_ = 0.1;
    float w_decay_ = 0.1;
    vec4 z_color_ = {1.0f, 0.0f, 0.00f, 0.3f};
    vec4 w_color_ = {0.0f, 1.0f, 0.00f, 0.3f};

    float depth_ = -0.7f;

    LatticeData(){}
};


class PointSet : public Lattice, public kipod::RenderObject
{
public:
    PointSet(Basis basis = Basis(), Sample sample = {-10,10,-10,10,-10,10,-10,10}) :
        Lattice(sample, basis)
    {
        LOG_INFO("Call PointSet Constructor with sample_size data {}, {} ,{}, {} ,{} ,{} ,{}, {}",
            sample_size_[0],sample_size_[1],sample_size_[2],sample_size_[3],sample_size_[4],sample_size_[5],
                sample_size_[6],sample_size_[7]);
        LOG_INFO("...and basis {}", basis_);
    }

    virtual void Init();
    virtual void Draw();

    void BaseChange(Basis new_basis);
    void UpdatePoints();
public:
    std::unique_ptr<LatticeData> lattice_data_= nullptr;

};

}