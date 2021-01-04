#include "../include/guimathcontrol.h"
#include "gui.h"



//void basis4AsFloatAndStringArray(Lattice& L, float* f, string* b){
//    mat4 currBasis = L.getBasisMatrix();
//    float* a = currBasis.data();
//    std::copy(a, a+16, f);
//    for(int j =0; j<16; j++) b[j] = std::to_string(*(f+j));
//}

//void sl2AsFloatArray(mat2 local_sl2, float *sl2_print){
//    float* h = local_sl2.data();
//    std::copy(h, h+4, sl2_print);
//}

void embedding_rules(std::array<int, 16> &selected, sl2Embedding &currEmbedding){
      if(selected[0] && selected[1] && selected[4] && selected[5])
                currEmbedding = TOP_LEFT;
        else if(selected[0] && selected[3] && selected[12] && selected[15])
                currEmbedding = OUTER;
        else if(selected[10] && selected[11] && selected[14] && selected[15])
                currEmbedding = BOTTOM_RIGHT;
        else if(selected[2] && selected[3] && selected[6] && selected[7])
                currEmbedding = TOP_RIGHT;
        else if(selected[8] && selected[9] && selected[12] && selected[13])
                currEmbedding = BOTTOM_LEFT;
        else if(selected[0] && selected[2] && selected[8] && selected[10])
                currEmbedding = XZ;
        else if(selected[5] && selected[6] && selected[9] && selected[10])
                currEmbedding = YZ;
        else if(selected[5] && selected[7] && selected[13] && selected[15])
                currEmbedding = YW;
        else currEmbedding = NOT_DEFINED;

        static int e = 0;
        e = static_cast<int>(currEmbedding);
}


int tl_arr[4] = {0,1,4,5};
int outer_arr[4] = {0,3,12,15};
int br_arr[4] = {10,11,14,15};
int tr_arr[4] = {2,3,6,7};
int bl_arr[4] = {8,9,12,13};
int xz_arr[4] = {0,2,8,10};
int yz_arr[4] = {5,6,9,10};
int yw_arr[4] = {5,7,13,15};
int nd_arr[4] = {-1,-1,-1,-1};

EmbeddingDescription tl(TOP_LEFT, tl_arr, "Top Left");
EmbeddingDescription outer(OUTER, outer_arr, "Outer");
EmbeddingDescription br(BOTTOM_RIGHT, br_arr, "Bottom Right");
EmbeddingDescription tr(TOP_RIGHT, tr_arr, "Top Right");
EmbeddingDescription bl(BOTTOM_LEFT, bl_arr, "Bottom Left");
EmbeddingDescription xz(XZ, xz_arr, "x-z");
EmbeddingDescription yz(YZ, yz_arr, "y-z");
EmbeddingDescription yw(YW, yw_arr, "y-w");
EmbeddingDescription nd(NOT_DEFINED, nd_arr, "???");

vector<EmbeddingDescription> allembeddings = {tl, outer, br, tr, bl ,xz, yz, yw, nd};

void embedding_rules_reverse(std::array<int, 16> &selected, sl2Embedding &currEmbedding){
        for(auto& emb: allembeddings){
            if(emb.embed == currEmbedding){
                for(int i =0; i<16; ++i){
                    if(i == emb.pos[0] || i == emb.pos[1] || i == emb.pos[2] || i == emb.pos[3]) selected[i] = 1;
                    else selected[i] = 0;
                }
                break;
            }
        }
}











void GUIMathControl::SetMat4(mat4& m){
        ImGui::Text("Basis:");
        ImGui::Columns(4, "mycolumns"); // 4-ways, with border
        ImGui::Separator();

        for (int i = 0; i < 4; i++)
        {
            for(int j =0; j <4; j++){

                ImGui::Text("%.2f", m[i][j]);
                ImGui::NextColumn();
            }
        }
        ImGui::Columns(1);

}
void GUIMathControl::matrix4i(int* f){
        ImGui::Text("Basis:");
        ImGui::Columns(4, "mycolumns"); // 4-ways, with border
        ImGui::Separator();

        for (int i = 0; i < 4; i++)
        {
            for(int j =0; j <4; j++){

                ImGui::Text("%i", *(f+(4*i+j)));
                ImGui::NextColumn();
            }
        }
        ImGui::Columns(1);

}

void GUIMathControl::mat4selectable(string* b, std::array<int, 16> &selected){
            ImGui::Separator();
            for (int i = 0; i < 4 * 4; i++)
                {
                    ImGui::PushID(i);
                    if (ImGui::Selectable(&b[i][0], selected[i] != 0, 0, ImVec2(50, 50)))
                    {
                        // Toggle
                        selected[i] = !selected[i];

                        // Note: We _unnecessarily_ test for both x/y and i here only to silence some static analyzer.
                        // The second part of each test is unnecessary.
                    }
                    if ((i % 4) < 3) ImGui::SameLine();
                    ImGui::PopID();
                }
     }

void GUIMathControl::embeddingsView(std::array<int, 16> &selected){
            ImGui::Separator();
            for (int i = 0; i < 4 * 4; i++)
                {
                    ImGui::PushID(i);
                    if(selected[i]){
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f));
                        ImGui::TextUnformatted("x");
                        ImGui::PopStyleColor();
                    }else ImGui::TextUnformatted("0");
                    if ((i % 4) < 3) ImGui::SameLine();
                    ImGui::PopID();
                }
     }

void GUIMathControl::embeddings(std::array<int, 16> &selected, sl2Embedding &currEmbedding){
    static int e = 0;
    ImGui::RadioButton("Top Left", &e, 0);
    ImGui::RadioButton("Outer", &e, 1);
    ImGui::RadioButton("Bottom Right", &e, 2);
    ImGui::RadioButton("Top Right", &e, 3);
    ImGui::NextColumn();
    ImGui::RadioButton("Bottom Left", &e, 4);
    ImGui::RadioButton("x-z", &e, 5);
    ImGui::RadioButton("y-z", &e, 6);
    ImGui::RadioButton("y-w", &e, 7);
    currEmbedding = static_cast<sl2Embedding>(e);
    embedding_rules_reverse(selected, currEmbedding);
}

void GUIMathControl::SetMat2(float* f){
      ImGui::Separator();
      for (int i = 0; i < 2; i++)
        {
            for(int j =0; j <2; j++){

                ImGui::Text("%.2f", *(f+(2*i+j)));
                ImGui::NextColumn();
            }
        }
        ImGui::Columns(1);
        ImGui::Separator();
     }

void GUIMathControl::verify4i(SampleSize &sampleSize, mode &currentMode){
     ImGui::Text("Size");
        ImGui::InputInt("x", &sampleSize.x);
        ImGui::InputInt("y", &sampleSize.y);
        ImGui::InputInt("z", &sampleSize.z);
        ImGui::InputInt("w", &sampleSize.w);
        static int clickedSampleSize = 0;
        if (ImGui::Button("Get new sample."))
            clickedSampleSize++;
        if (clickedSampleSize)
        {
            currentMode = NEW_SAMPLE;
            ImGui::SameLine();  ImGui::Text("calculating...");
            clickedSampleSize = 0;
        }
}

void GUIMathControl::matrix4(mat4& m){
    for (int i = 0; i < 4; i++){
            for(int j =0; j <4; j++){
                ImGui::Text("%.2f", m[i][j]);
                if(j<3) ImGui::SameLine();
            }
        }
}

void GUIMathControl::matrix(float* f, int size){
    for (int i = 0; i < size; i++){
            for(int j =0; j <size; j++){
                ImGui::Text("%.2f", *(f+(size*i+j)));
                if(j<size-1) ImGui::SameLine();
            }
        }
}



void GUIMathControl::sl4matrix(mat4& m){
    static float* f = &m[0][0];
    for (int i = 0; i < 4; i++){
            for(int j =0; j <4; j++){
                ImGui::Text("%.2f", *(f+(4*i+j)));
                if(j<3) ImGui::SameLine();
            }
        }
}


bool GUIMathControl::sl2matrix(float& s, SL2type sl2type, int id=1){
        switch(sl2type){
                case(DILATE):

                            ImGui::Text("Dilate");
                            ImGui::Text("%.2f", exp(s));
                            ImGui::SameLine();
                            ImGui::Text("%.2f", 0.0);
                            ImGui::Text("%.2f", 0.0);
                            ImGui::SameLine();
                            ImGui::Text("%.2f", exp(-s));
                            if(ImGui::SliderFloat("##float_dilate", &s, -5.0, 5.0)) return true;
                            break;

                case(SHEAR_U):
                            ImGui::Text("Shear");
                            ImGui::Text("%.2f", 1.0);
                            ImGui::SameLine();
                            ImGui::Text("%.2f", s);
                            ImGui::Text("%.2f", 0.0);
                            ImGui::SameLine();
                            ImGui::Text("%.2f", 1.0);
                            if(ImGui::SliderFloat("##float_shearU", &s, -10.0, 10.0)) return true;
                            break;

                case(SHEAR_L):
                            ImGui::Text("Shear");
                            ImGui::Text("%.2f", 1.0);
                            ImGui::SameLine();
                            ImGui::Text("%.2f", 0.0);
                            ImGui::Text("%.2f", s);
                            ImGui::SameLine();
                            ImGui::Text("%.2f", 1.0);
                            if(ImGui::SliderFloat("##float_shearL", &s, -10.0, 10.0)) return true;
                            break;

                case(ROTATE):
                            float angle = s * M_PI;
                            ImGui::PushID(id);
                            ImGui::Text("Rotate");
                            ImGui::Text("%.2f", cos(angle));
                            ImGui::SameLine();
                            ImGui::Text("%.2f", -sin(angle));
                            ImGui::Text("%.2f", sin(angle));
                            ImGui::SameLine();
                            ImGui::Text("%.2f", cos(angle));
                            if(ImGui::SliderFloat("##float_rotate", &s, 0.0, 1.0)) {ImGui::PopID(); return true;}
                            ImGui::PopID();
                            break;

            }
            return false;
}

void GUIMathControl::sl2control(mat4& temporaryMatrixView, MatrixWalk &SL4walk, sl2Embedding currEmbedding){
        static float dilate = 0;
        static float shearU = 0;
        static float shearL = 0;
        static float rotate1 = 0;
        static float rotate2 = 0;
        static vector<sl2insl4> initlocal= {
                    std::make_tuple(ROTATE, currEmbedding, rotate1),
                    std::make_tuple(SHEAR_L, currEmbedding, shearL),
                    std::make_tuple(DILATE, currEmbedding, dilate),
                    std::make_tuple(SHEAR_U, currEmbedding, shearU),
                    std::make_tuple(ROTATE, currEmbedding, rotate2)
                };
        static MatrixWalk LocalFive(initlocal);
        static mat4 total = mat4(1.0f);

        static bool immediate_mode = false;

        ImGui::Checkbox("Immediate Mode", &immediate_mode);
        ImGui::SameLine();
        if(ImGui::Button("Reset")){
                shearU=0; dilate =0; shearL = 0; rotate1 =0; rotate2=0;
                LocalFive.modify(0, std::make_tuple(ROTATE, currEmbedding, rotate1));
                LocalFive.modify(1, std::make_tuple(SHEAR_L, currEmbedding, shearL));
                LocalFive.modify(2, std::make_tuple(DILATE, currEmbedding, dilate));
                LocalFive.modify(3, std::make_tuple(SHEAR_U, currEmbedding, shearU));
                LocalFive.modify(4, std::make_tuple(ROTATE, currEmbedding, rotate2));
        }
        ImGui::SameLine();
        if(currEmbedding == TOP_LEFT) ImGui::Text("Top Left");
        else if (currEmbedding == OUTER) ImGui::Text("Outer");
        else if (currEmbedding == BOTTOM_RIGHT) ImGui::Text("Bottom Right");
        else if (currEmbedding == TOP_RIGHT) ImGui::Text("Top Right");
        else if (currEmbedding == BOTTOM_LEFT) ImGui::Text("Bottom Left");
        else if (currEmbedding == XZ) ImGui::Text("x-z");
        else if (currEmbedding == YZ) ImGui::Text("y-z");
        else if (currEmbedding == YW) ImGui::Text("z-w");
        else ImGui::Text("Embedding?");


        ImGui::Columns(5, NULL, true);

        {
                if(sl2matrix(rotate1, ROTATE) && immediate_mode) LocalFive.modify(0, std::make_tuple(ROTATE, currEmbedding, rotate1));
                if(ImGui::Button("Add##Rotate1"))		SL4walk.append(std::make_tuple(ROTATE, currEmbedding, rotate1));
                ImGui::SameLine();
                if(ImGui::Button("0##Rotate1")) {rotate1=0;LocalFive.modify(0, std::make_tuple(ROTATE, currEmbedding, rotate1));		}
                ImGui::NextColumn();
        }
        {
                if(sl2matrix(shearL, SHEAR_L) && immediate_mode) LocalFive.modify(1, std::make_tuple(SHEAR_L, currEmbedding, shearL));
                if(ImGui::Button("Add##ShearL"))		SL4walk.append(std::make_tuple(SHEAR_L, currEmbedding, shearL));
                ImGui::SameLine();
                if(ImGui::Button("0##ShearL1")) {shearL=0;	LocalFive.modify(1, std::make_tuple(SHEAR_L, currEmbedding, shearL));		}
                ImGui::NextColumn();
        }

        {
                if(sl2matrix(dilate, DILATE) && immediate_mode) LocalFive.modify(2, std::make_tuple(DILATE, currEmbedding, dilate));
                if(ImGui::Button("Add##Dilate"))		SL4walk.append(std::make_tuple(DILATE, currEmbedding, dilate));
                ImGui::SameLine();
                if(ImGui::Button("0##dilate")) {dilate=0;	           LocalFive.modify(2, std::make_tuple(DILATE, currEmbedding, dilate)); 	}
                ImGui::NextColumn();
        }
        {
                if(sl2matrix(shearU, SHEAR_U) && immediate_mode) LocalFive.modify(3, std::make_tuple(SHEAR_U, currEmbedding, shearU));
                if(ImGui::Button("Add##ShearU"))		SL4walk.append(std::make_tuple(SHEAR_U, currEmbedding, shearU));
                ImGui::SameLine();
                if(ImGui::Button("0##shearU")) {shearU=0;	           LocalFive.modify(3, std::make_tuple(SHEAR_U, currEmbedding, shearU));	 	}
                ImGui::NextColumn();
        }
        {
                if(sl2matrix(rotate2, ROTATE, 2) && immediate_mode) LocalFive.modify(4, std::make_tuple(ROTATE, currEmbedding, rotate2));
                if(ImGui::Button("Add##Rotate2"))		SL4walk.append(std::make_tuple(ROTATE, currEmbedding, rotate2));
                ImGui::SameLine();
                if(ImGui::Button("0##rotate2")) {rotate2=0;	  LocalFive.modify(4, std::make_tuple(ROTATE, currEmbedding, rotate2));        		}
                ImGui::NextColumn();
        }
        ImGui::Separator();
        for(auto n : LocalFive.sl4stack){
            matrix4(n);
            ImGui::NextColumn();
        }
        ImGui::Columns(2);
        ImGui::Text("Total:");
        total = LocalFive.multiplyAll();
        if(immediate_mode) temporaryMatrixView=total;
        if(ImGui::Button("Add##total"))	SL4walk.append(total);
        ImGui::NextColumn();
        matrix4(total);

        ImGui::Columns(1);
}

void GUIMathControl::sl4control(MatrixWalk &SL4walk){

        if(SL4walk.sl4stack.empty()) ;
        else{
        ImGui::Columns(5, NULL, true);
        for(auto n : SL4walk.sl4stack){
            matrix4(n);
            ImGui::NextColumn();
        }
        }
        ImGui::Columns(1);

        // static float dilate = 1;
     // 	static float shearU = 0;
     // 	static float shearL = 0;
     // 	static float rotate = 0;

     // 	static bool immediate_mode = false;

     // 	static bool h_borders = true;
  //       static bool v_borders = true;
  //       static int columns_count = 4;
  //       const int lines_count = 3;
  //       ImGui::SetNextItemWidth(ImGui::GetFontSize() * 8);
  //       ImGui::DragInt("##columns_count", &columns_count, 0.1f, 2, 10, "%d columns");
  //       if (columns_count < 2)
  //           columns_count = 2;
  //       ImGui::SameLine();
  //       ImGui::Checkbox("horizontal", &h_borders);
  //       ImGui::SameLine();
  //       ImGui::Checkbox("vertical", &v_borders);
  //       ImGui::SameLine();
  //       ImGui::Checkbox("Immediate Mode", &immediate_mode);
  //       ImGui::SameLine();
  //      	if(currEmbedding == TOP_LEFT) ImGui::Text("Top Left");
  //       else if (currEmbedding == OUTER) ImGui::Text("Outer");
  //       else if (currEmbedding == BOTTOM_RIGHT) ImGui::Text("Bottom Right");
  //       else if (currEmbedding == TOP_RIGHT) ImGui::Text("Top Right");
  //       else if (currEmbedding == BOTTOM_LEFT) ImGui::Text("Bottom Left");
  //       else if (currEmbedding == XZ) ImGui::Text("x-z");
  //       else if (currEmbedding == YZ) ImGui::Text("y-z");
  //       else if (currEmbedding == YW) ImGui::Text("z-w");
  //       else ImGui::Text("Embedding?");

  //       ImGui::Columns(columns_count, NULL, v_borders);

  //       {
  //           if (h_borders && ImGui::GetColumnIndex() == 0)
  //               ImGui::Separator();
  //         		sl2matrix(dilate, DILATE);
  //         		if(ImGui::Button("Apply##Dilate") || immediate_mode)		SL2stack.push_back(std::make_tuple(DILATE, currEmbedding, dilate));
  //           	ImGui::NextColumn();
  //       }
  //       {
  //           if (h_borders && ImGui::GetColumnIndex() == 0)
  //               ImGui::Separator();
  //         		sl2matrix(shearU, SHEAR_U);
  //         		if(ImGui::Button("Apply##ShearU") || immediate_mode)		SL2stack.push_back(std::make_tuple(SHEAR_U, currEmbedding, shearU));
  //           	ImGui::NextColumn();
  //       }
  //       {
  //           if (h_borders && ImGui::GetColumnIndex() == 0)
  //               ImGui::Separator();
  //         		sl2matrix(shearL, SHEAR_L);
  //         		if(ImGui::Button("Apply##ShearL") || immediate_mode)		SL2stack.push_back(std::make_tuple(SHEAR_L, currEmbedding, shearL));
  //           	ImGui::NextColumn();
  //       }
  //       {
  //           if (h_borders && ImGui::GetColumnIndex() == 0)
  //               ImGui::Separator();
  //         		sl2matrix(rotate, ROTATE);
  //         		if(ImGui::Button("Apply##Rotate") || immediate_mode)		SL2stack.push_back(std::make_tuple(ROTATE, currEmbedding, rotate));
  //           	ImGui::NextColumn();
  //       }


  //       for (int i = 4; i < columns_count * lines_count; i++)
  //       {
  //           if (h_borders && ImGui::GetColumnIndex() == 0)
  //               ImGui::Separator();
  //           ImGui::Text("%c%c%c", 'a' + i, 'a' + i, 'a' + i);
  //           ImGui::Text("Width %.2f", ImGui::GetColumnWidth());
  //           ImGui::Text("Avail %.2f", ImGui::GetContentRegionAvail().x);
  //           ImGui::Text("Offset %.2f", ImGui::GetColumnOffset());
  //           ImGui::Text("Long text that is likely to clip");
  //           ImGui::Button("Button", ImVec2(-FLT_MIN, 0.0f));
  //           ImGui::NextColumn();
  //       }
  //       ImGui::Columns(1);
  //       if (h_borders)
  //       ImGui::Separator();

}


void GUIMathControl::drawHull(Vectors2 hull){

        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        static ImVec4 colf = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
        const ImU32 col = ImColor(colf);
        float th = 2.0f;
         static float sz = 36.0f;

        ImVec2 p = ImGui::GetCursorScreenPos();
        p.x = p.x + 4.0f, p.y = p.y + 4.0f;
        for(auto it = begin(hull), e = end(hull); it!= e-1; ++it){
            ImVec2 s = ImVec2( p.x+(*it)[0], p.y+(*it)[1] );
            ImVec2 t = ImVec2( p.x+(*(it+1))[0], p.y+(*(it+1))[1] );
            draw_list->AddLine(s, t, col, th);
        }
        for(auto& v: hull){
                ImGui::Text("%f,%f ", v[0],v[1]);
        }


}


GUIMathControl::GUIMathControl()
{
}
