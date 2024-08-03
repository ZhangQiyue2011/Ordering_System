#include <iostream>
#include <imgui.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#define STB_IMAGE_IMPLEMENTATION  
#include "stb_image.h"  
using namespace std;

struct Food
{
    char s[60];
    float prize;
    char unit[8];
    unsigned short num = 0;
};

bool shouldQuit;
//Food foods[100] = {{"面条", 3.5, "碗", 0}, {"豆浆", 2.2, "杯", 0}, {"米饭", 1.3, "份", 0}, {"牛奶", 1, "盒", 0}, {"西红柿炒鸡蛋盖浇饭", 3.0, "份", 0}， {"", 0, "", 0}};
//int food_cnt = 5;
Food foods[100] = {};
int food_cnt = 0;
char target[100], target2[30];
float prize_all = 0;

GLuint LoadTextureFromFile(const char* filename, int* out_width, int* out_height) {  
    GLuint textureID;  
    glGenTextures(1, &textureID);  
    glBindTexture(GL_TEXTURE_2D, textureID);  
  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  
  
    int width, height, nrChannels;  
    stbi_set_flip_vertically_on_load(false);
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);  
    if (data) {  
        GLenum format;  
        if (nrChannels == 1)  
            format = GL_RED;  
        else if (nrChannels == 3)  
            format = GL_RGB;  
        else if (nrChannels == 4)  
            format = GL_RGBA;  
  
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);  
        glGenerateMipmap(GL_TEXTURE_2D);  
    } else {  
        std::cerr << "Failed to load texture: " << filename << std::endl;  
        return 0;  
    }  
    stbi_image_free(data);  
  
    *out_width = width;  
    *out_height = height;  
  
    return textureID;  
}  

bool start_Food(void)
{
    FILE *fl = fopen("load_food.txt", "r");
    if (!fl)
    {
        fprintf(stderr, "打开文件失败。");
        return true;
    }
    while (true)
    {
        int got = fscanf(fl, "%s %f %s", foods[food_cnt].s, &foods[food_cnt].prize, foods[food_cnt].unit);
        if (got == EOF)
        {
            break;
        }
        foods[food_cnt++].num = 0;
    }
    return false;
}

int main(int argc, char const *argv[])
{
    bool state = start_Food();
    if (state)
    {
        return -1;
    }
    GLFWwindow* window;  
  
    if (!glfwInit()) {  
        fprintf(stderr, "Failed to initialize GLFW\n");  
        return -1;  
    }  
  
    window = glfwCreateWindow(625, 710, "xx饭店订餐系统", NULL, NULL);  
    if (!window) {  
        glfwTerminate();  
        fprintf(stderr, "Failed to create GLFW window\n");  
        return -1;  
    }  
    glfwMakeContextCurrent(window);  
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); // 初始化 Glad  

  
    ImGui::CreateContext();  
    ImGuiIO& io = ImGui::GetIO();  
    // 加载中文字体  
    ImFontConfig fontConfig;  
    fontConfig.MergeMode = false; 
    io.Fonts->AddFontFromFileTTF("shiweiyongtuiheixiti.ttf", 16.0f, &fontConfig, io.Fonts->GetGlyphRangesChineseFull());  
    ImGui_ImplGlfw_InitForOpenGL(window, true);  
    ImGui_ImplOpenGL3_Init("#version 330 core");  
    ImGui::StyleColorsLight();

    GLuint textureID = 0;  
    int texture_width = 0, texture_height = 0;  
    textureID = LoadTextureFromFile("sunflower.png", &texture_width, &texture_height);  
  
    while (!glfwWindowShouldClose(window) && !shouldQuit) {  
        glfwPollEvents();
        
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Background Window", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoCollapse);  
        ImGui::Image((void*)(intptr_t)textureID, ImVec2(texture_width * 1.25, texture_height * 1.25));  
        ImGui::End();  
        
        ImGui::Begin("订餐");
        ImGui::Text("这里是订餐系统，请您点餐！");
        for (int i = 0; i < food_cnt; ++i)
        {
            snprintf(target, sizeof(target), "%s（价格：%.2f元/%s，点了%d%s，需要花%.2f元）", foods[i].s, foods[i].prize, foods[i].unit,
                                                                                         foods[i].num, foods[i].unit, foods[i].prize * foods[i].num);
            if (ImGui::Button(target))
            {
                ++foods[i].num;
                prize_all += foods[i].prize;
            }
            snprintf(target, sizeof(target), "%s-", foods[i].s);
            if (ImGui::Button(target))
            {
                if (foods[i].num != 0)
                {
                    --foods[i].num;
                    prize_all -= foods[i].prize;
                }
            }
        }
        snprintf(target2, sizeof(target2), "交钱（总价%.2f元）", prize_all);
        if (ImGui::Button(target2))
        {
            shouldQuit = true;
        }
        ImGui::End();
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);  
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    
    glfwTerminate();
    return 0;
}
