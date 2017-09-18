#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    window_width = 1040;
    window_height = 720;
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //required call
    gui.setup();
    
    //------ SETUP NODE GRAPH
    calcGraphType = new gtf::NodeGraphType(GTF_UID("CalcGraphType"));

    auto numberDesc = new gtf::NodeConnectionDesc<gtf::NodeConnectionI32>(GTF_UID("NumberConnection"), "Value");
    auto inputADesc = new gtf::NodeConnectionDesc<gtf::NodeConnectionI32>(GTF_UID("NumberInputConnectionA"), "InputA");
    auto inputBDesc = new gtf::NodeConnectionDesc<gtf::NodeConnectionI32>(GTF_UID("NumberInputConnectionB"), "InputB");
    
    gtf::NodeTypeBase* nodeType = nullptr;
    nodeType = new gtf::NodeType<CalcNumberNode>(GTF_UID("CalcNumberNode"), "Number");
    nodeType->outputConnectionsDesc.push_back(numberDesc);
    calcGraphType->registerNodeType(nodeType);
    
    nodeType = new gtf::NodeType<CalcAddNode>(GTF_UID("CalcAddNode"), "Add");
    nodeType->inputConnectionsDesc.push_back(inputADesc);
    nodeType->inputConnectionsDesc.push_back(inputBDesc);
    nodeType->outputConnectionsDesc.push_back(numberDesc);
    calcGraphType->registerNodeType(nodeType);
    
    nodeType = new gtf::NodeType<CalcSubstractNode>(GTF_UID("CalcSubstractNode"), "Substract");
    nodeType->inputConnectionsDesc.push_back(inputADesc);
    nodeType->inputConnectionsDesc.push_back(inputBDesc);
    nodeType->outputConnectionsDesc.push_back(numberDesc);
    calcGraphType->registerNodeType(nodeType);
    
    nodeType = new gtf::NodeType<CalcMultiplyNode>(GTF_UID("CalcMultiplyNode"), "Multiply");
    nodeType->inputConnectionsDesc.push_back(inputADesc);
    nodeType->inputConnectionsDesc.push_back(inputBDesc);
    nodeType->outputConnectionsDesc.push_back(numberDesc);
    calcGraphType->registerNodeType(nodeType);
    
    nodeType = new gtf::NodeType<CalcDivideNode>(GTF_UID("CalcDivideNode"), "Divide");
    nodeType->inputConnectionsDesc.push_back(inputADesc);
    nodeType->inputConnectionsDesc.push_back(inputBDesc);
    nodeType->outputConnectionsDesc.push_back(numberDesc);
    calcGraphType->registerNodeType(nodeType);
    
    calcGraphInstance = new CalcNodeGraph(calcGraphType);
    
    //gtf::GRHI->setClearColor(0.3f, 0.3f, 0.28f, 1.0f);
    
    ofClear(76,76,71,255);
}

//--------------------------------------------------------------
void ofApp::update(){
    calcGraphInstance->updateNodes();
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    //--------------required to call this at beginning
    this->gui.begin();
    
    ImGuiWindowFlags flags = 0;
    flags |= ImGuiWindowFlags_NoCollapse;
    flags |= ImGuiWindowFlags_NoMove;
    flags |= ImGuiWindowFlags_NoResize;
    flags |= ImGuiWindowFlags_NoTitleBar;
    flags |= ImGuiWindowFlags_HorizontalScrollbar;
    
    float propertiesWith = 250;
    
    ImGui::SetNextWindowPos(ImVec2(window_width - propertiesWith - 20.0f, 20.0f), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(propertiesWith, window_height - 240.0f), ImGuiSetCond_Always);
    
    ImGui::Begin("NodeProperties", nullptr, flags);
    {
        ImGui::Text("Node Properties");
        ImGui::Separator();
        
        if(calcGraphInstance->selectedNodes.size() == 1)
        {
            CalcNode* node = CalcNode::CAST(calcGraphInstance->selectedNodes.front());
            if(node && node->type == ECalcNodeType::CNT_NUMBER)
            {
                
                node->dirty = ImGui::InputInt("Value", &node->number);
            }
            
        }
    }
    ImGui::End();
    
    ImGui::SetNextWindowPos(ImVec2(window_width - propertiesWith - 20.0f, window_height - 200.0f), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(propertiesWith, 180.0f), ImGuiSetCond_Always);
    
    if(ImGui::Begin("Controls", nullptr, flags))
    {
        ImGui::Text("Controls");
        ImGui::Separator(); ImGui::Spacing();
        
        ImGui::Text("Right-Click to open menu."); ImGui::Spacing();
        ImGui::Text("Drag connections to link."); ImGui::Spacing();
        ImGui::Text("[Shit]+Click on a node for multi \nselection."); ImGui::Spacing();
        ImGui::Text("[Ctrl]+Click on a connection \nto break links."); ImGui::Spacing();
        ImGui::Text("[Supr] to delete selected nodes"); ImGui::Spacing();
        
    }
    ImGui::End();
    
    ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Always);
    ImGui::SetNextWindowSize(ImVec2(window_width - propertiesWith - 60.0f, window_height - 40.0f), ImGuiSetCond_Always);
    
    if(ImGui::Begin("Graph", nullptr, flags))
    {
        calcGraphInstance->updateGUI();
    }
    ImGui::End();
    
    
    //-------------required to call this at end
    this->gui.end();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
