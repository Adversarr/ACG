#include "app.hpp"

namespace acg::app {

void HybredApp::Step() {

}


void HybredApp::Init() {
  cloth_.InitAuxiliary();
  softbody_.InitAuxiliary();
}
}
