#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    window_width = ofGetWidth();
    window_height = ofGetHeight();
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    
    //required call
    gui.setup();
    
    //------ SETUP NODE GRAPH
    calcGraphType = new gtf::NodeGraphType(GTF_UID("CalcGraphType"));

    auto numberDesc = new gtf::NodeConnectionDesc<gtf::NodeConnectionF32>(GTF_UID("NumberConnection"), "Value");
    auto inputADesc = new gtf::NodeConnectionDesc<gtf::NodeConnectionF32>(GTF_UID("NumberInputConnectionA"), "InputA");
    auto inputBDesc = new gtf::NodeConnectionDesc<gtf::NodeConnectionF32>(GTF_UID("NumberInputConnectionB"), "InputB");
    
    gtf::NodeTypeBase* nodeType = nullptr;
    nodeType = new gtf::NodeType<CalcNumberNode>(GTF_UID("CalcNumberNode"), "Number");
    nodeType->outputConnectionsDesc.push_back(numberDesc);
    calcGraphType->registerNodeType(nodeType);
    
    //--- MATH OPS
    nodeType = new gtf::NodeType<CalcAddNode>(GTF_UID("CalcAddNode"), "Add");
    nodeType->inputConnectionsDesc.push_back(inputADesc);
    nodeType->inputConnectionsDesc.push_back(inputBDesc);
    nodeType->outputConnectionsDesc.push_back(numberDesc);
    calcGraphType->registerNodeType(nodeType);
    
    nodeType = new gtf::NodeType<CalcSubstractNode>(GTF_UID("CalcSubstractNode"), "Subtract");
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
    
    //--- WAVEFORMS
    nodeType = new gtf::NodeType<SignalSineNode>(GTF_UID("SignalSineNode"), "Sine");
    nodeType->outputConnectionsDesc.push_back(numberDesc);
    calcGraphType->registerNodeType(nodeType);
    
    nodeType = new gtf::NodeType<SignalTriangleNode>(GTF_UID("SignalTriangleNode"), "Triangle");
    nodeType->outputConnectionsDesc.push_back(numberDesc);
    calcGraphType->registerNodeType(nodeType);
    
    nodeType = new gtf::NodeType<SignalSquareNode>(GTF_UID("SignalSquareNode"), "Square");
    nodeType->outputConnectionsDesc.push_back(numberDesc);
    calcGraphType->registerNodeType(nodeType);
    
    nodeType = new gtf::NodeType<SignalSawNode>(GTF_UID("SignalSawNode"), "Sawtooth");
    nodeType->outputConnectionsDesc.push_back(numberDesc);
    calcGraphType->registerNodeType(nodeType);
    
    nodeType = new gtf::NodeType<SignalNoiseNode>(GTF_UID("SignalNoiseNode"), "Noise");
    nodeType->outputConnectionsDesc.push_back(numberDesc);
    calcGraphType->registerNodeType(nodeType);
    
    
    //--- OUTPUT OPS
    nodeType = new gtf::NodeType<CalcOutputNode>(GTF_UID("CalcOutputNode"), "Output");
    nodeType->inputConnectionsDesc.push_back(inputADesc);
    calcGraphType->registerNodeType(nodeType);
    
    calcGraphInstance = new CalcNodeGraph(calcGraphType);
}

//--------------------------------------------------------------
void ofApp::update(){
    calcGraphInstance->updateNodes();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(76,76,71,255);

    //--------------required to call this at beginning
    this->gui.begin();
    
    OutputNode* output_node = OutputNode::CAST(calcGraphInstance->selectedNodes.front());
    if(output_node && output_node->type == ECalcNodeType::OUTPUT){
        float val = output_node->number;
        cout << "val = " << val;
        ofBackground(val,255);
    }
    
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
                node->dirty = ImGui::InputFloat("Value", &node->number);
            }
            
            SignalNode* signal_node = SignalNode::CAST(calcGraphInstance->selectedNodes.front());
            if(signal_node && signal_node->type == ECalcNodeType::WAVEFORM)
            {
                signal_node->dirty = ImGui::InputFloat("Speed", &signal_node->speed);
                signal_node->dirty = ImGui::InputFloat("Amp Min", &signal_node->amp_min);
                signal_node->dirty = ImGui::InputFloat("Amp Max", &signal_node->amp_max);
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
    window_width = w;
    window_height = h;
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
