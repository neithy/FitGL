#include "TimingMonitorGL.h"
#include "bunny.h"
#include "BaseApp.h"
#include "ShaderUtils.h"
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace fit;
using namespace std;
using namespace ge::gl;

char* compSrc = R".(
#version 450
layout(local_size_x = 1, local_size_y = 32, local_size_z = 1) in;

layout(binding = 0) uniform writeonly image2D img;

layout(binding = 0, std430) buffer dataBuffer{
  float data[];
};

uniform int historyLength=200;
uniform int intervals=1;
uniform int offset=0;
uniform float maxVal=20;

uniform vec3 colors[8];

void main(){
  ivec2 id = ivec2(gl_GlobalInvocationID.xy);
  if(id.y>imageSize(img).y) return;


  float tr = 1-float(id.y)/imageSize(img).y;
  vec3 color = vec3(0,0,0);

  int size = historyLength*intervals;
  
  for(int i=intervals-1;i>=0;i--){
    int index = (id.x*intervals+i+offset)%size;
    float val = data[index];
    if(tr<=val/maxVal){
      color = colors[i%8];
    }
  }
  imageStore(img,id,vec4(color,0.5)); 
}

).";

glm::vec3 colors[] = {
  glm::vec3(0.2, 0.5, 1),
  glm::vec3(1, 0, 0),
  glm::vec3(0, 1, 0),
  glm::vec3(0, 1, 1),
  glm::vec3(1, 0, 1),
  glm::vec3(1, 1, 0),
  glm::vec3(1, 1, 1),
  glm::vec3(0, 0, 1),
  glm::vec3(0.2, 0.5, 1)
};

TimingMonitorGL::TimingMonitorGL(int intervals, int historyLength): historyLength(historyLength), intervals(intervals) {
  //data.resize(intervals*historyLength);
  queries.resize(intervals + 1);
  glGenQueries(intervals + 1, queries.data());

  texture = make_shared<Texture>(GL_TEXTURE_2D, GL_RGBA, 0, historyLength, 128);
  texture->texParameteri(GL_TEXTURE_WRAP_R, GL_REPEAT);
  texture->texParameteri(GL_TEXTURE_WRAP_S, GL_REPEAT);
  texture->texParameteri(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  texture->texParameteri(GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  auto cs = make_shared<Shader>(GL_COMPUTE_SHADER, compSrc);
  program = createProgram(cs);

  names.resize(intervals);

  buffer = make_shared<Buffer>();
  dataLength = intervals * historyLength;
  buffer->alloc(dataLength * sizeof(float), 0, GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
  data = (float*)buffer->map(0, buffer->getSize(), GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT);
  assert(data);
}

TimingMonitorGL::~TimingMonitorGL() {
}

void TimingMonitorGL::startFrame() {
  frameTimestamps = 0;
  glQueryCounter(queries[frameTimestamps], GL_TIMESTAMP);
  offset -= intervals;
  if(offset < 0)offset += dataLength;
}

void TimingMonitorGL::timestamp(std::string name) {
  frameTimestamps++;
  if(frameTimestamps > intervals)return;
  glQueryCounter(queries[frameTimestamps], GL_TIMESTAMP);
  names[frameTimestamps - 1] = name;
}

void TimingMonitorGL::draw() {
  GLuint64 timeStart;
  glGetQueryObjectui64v(queries[0], GL_QUERY_RESULT, &timeStart);

  for(int i = 0; i < intervals; i++) {
    GLuint64 timestamp;
    glGetQueryObjectui64v(queries[1 + i], GL_QUERY_RESULT, &timestamp);
    float diff = (timestamp - timeStart) * 1e-6; // ms
    data[offset + i] = diff;
  }

  float maxVal = 0;
  for(int i = 0; i < dataLength; i++) {
    maxVal = (std::max)(maxVal, data[i]);
  }
  maxVal = ceil(maxVal / 20.0) * 20;

  program->use();
  program->set1i("historyLength", historyLength);
  program->set1i("intervals", intervals);
  program->set1i("offset", offset);
  program->set1f("maxVal", maxVal);
  program->set3fv("colors", (float*)colors, 8);

  texture->bindImage(0, 0, GL_RGBA8,GL_WRITE_ONLY);
  buffer->bindBase(GL_SHADER_STORAGE_BUFFER, 0);
  glDispatchCompute(historyLength, ceil(texture->getHeight(0) / 32.0), 1);

  ImGui::Begin("Timing", 0, ImVec2(historyLength * 2, 200), 0.0f,
               ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
               ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
               ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs);
  ImGui::SetWindowPos(ImVec2(10, 30));
  ImGui::Image((ImTextureID)texture->getId(), ImVec2(texture->getWidth(0), texture->getHeight(0)), ImVec2(0, 0), ImVec2(-1, 1));

  for(int i = 0; i < intervals+1; i++) {
    stringstream stream;
    if(i==intervals) {
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1,1,1, 1));
      stream << setw(5) << fixed << setprecision(2) << data[offset + i-1] << " " << "Total";
    }else{
      ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(colors[i].x, colors[i].y, colors[i].z, 1));
      float val = data[offset + i]; 
      if (i > 0) val -= data[offset + i - 1];
      stream << setw(5)<< fixed << setprecision(2) << val << " " <<names[i];
    }
    label(stream.str().c_str(), 20 + historyLength, 30 + (intervals-i) * 12);
    ImGui::PopStyleColor();
  }

  ImGui::End();


}
