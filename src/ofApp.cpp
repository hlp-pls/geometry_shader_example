#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //ofSetLogLevel(OF_LOG_VERBOSE);
    glEnable(GL_DEPTH_TEST);
    ofSetVerticalSync(false);
    
    sh_p_render.setGeometryInputType(GL_LINES_ADJACENCY);
    sh_p_render.setGeometryOutputType(GL_TRIANGLE_STRIP);
    sh_p_render.setGeometryOutputCount(4);
    
    sh_p_render.load("shaders/p_render.vert",
                     "shaders/p_render.frag",
                     "shaders/p_render.geom");
    sh_p_pos.load("shaders/p_pos.vert","shaders/p_pos.frag");
   
    tex_w = 4;
    tex_h = 4;
    c_num = 8;
    
    vector<float> init_pos(tex_w * tex_h * c_num * 3);
    particles_mesh.setMode(OF_PRIMITIVE_LINES_ADJACENCY);
    //particles_mesh.enableIndices();
    //particles_mesh.enableTextures();
    
    float _x = 0;
    float _y = 0;
    float _z = 0;
    
    for(int y = 0; y < tex_h; y++){
        for(int x = 0; x < tex_w; x++){
            
            for(int k = 0; k < c_num; k++){
                
                int i = ( tex_w * y + x ) * c_num + k;
                
                if(k == 0 || k % 4 == 2)
                {
                    _x = ofRandom(1.0);
                    _y = ofRandom(1.0);
                    //_z = ofRandom(-10.0,0.0);
                }
                    
                {
                    init_pos[i*3+0] = _x; //x
                    init_pos[i*3+1] = _y; //y
                    init_pos[i*3+2] = _z; //z
                }
                
                particles_mesh.addVertex({ x * c_num + k, y, 0 });
                //particles_mesh.addTexCoord({ x * c_num + k, y });
                //particles_mesh.addIndex(k);
            }
        }
    }
    
    for(int y = 0; y < tex_h; y++){
        for(int x = 0; x < tex_w; x++){
            for(int k = 0; k < c_num; k++){
                
                int i = ( tex_w * y + x ) * c_num + k;
                
                if( k != 0 && k % 4 == 0 ){
                    init_pos[i*3+0] = init_pos[(i-3)*3+0]; //x
                    init_pos[i*3+1] = init_pos[(i-3)*3+1]; //y
                    init_pos[i*3+2] = init_pos[(i-3)*3+2]; //z
                }
                
                if( k < c_num - 4 && k % 4 == 3 ){
                    init_pos[i*3+0] = init_pos[(i+3)*3+0]; //x
                    init_pos[i*3+1] = init_pos[(i+3)*3+1]; //y
                    init_pos[i*3+2] = init_pos[(i+3)*3+2]; //z
                }
                
            }
        }
    }
    
    //sh_p_render.printActiveAttributes();
    //ofLog() << sh_p_render.getAttributeLocation("position");
    
    particles_p_PingPong.allocate(tex_w * c_num, tex_h, GL_RGB32F);
    particles_p_PingPong.src->getTexture().loadData(init_pos.data(), tex_w * c_num, tex_h, GL_RGB);
    particles_p_PingPong.dst->getTexture().loadData(init_pos.data(), tex_w * c_num, tex_h, GL_RGB);
    
    fbo_render.allocate(ofGetWidth(), ofGetHeight(), GL_RGB32F);
    fbo_render.begin();
    ofClear(0,0,0,255);
    fbo_render.end();
}

//--------------------------------------------------------------
void ofApp::update(){
    
    glm::mat4 projection;
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 model = glm::mat4(1.0f);
    
    float fov_angle = 45.0f;
    float camera_distance = (2.0 * 0.5) / tan(glm::radians(fov_angle * 0.5));
    
    projection = glm::perspective(glm::radians(fov_angle), (float)ofGetWidth() / (float)ofGetHeight(), 0.1f, 100.0f);
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -camera_distance));
    
    
    particles_p_PingPong.dst->begin();
    ofClear(0);
    {
        sh_p_pos.begin();
        
        sh_p_pos.setUniformMatrix4f("projection", projection);
        sh_p_pos.setUniformMatrix4f("view", view);
        sh_p_pos.setUniformMatrix4f("model", model);
        
        sh_p_pos.setUniformTexture("pos_texture", particles_p_PingPong.src->getTexture(), 1);
        sh_p_pos.setUniform2f("pos_res", (float)particles_p_PingPong.src->getTexture().getWidth(), (float)particles_p_PingPong.src->getTexture().getHeight());
        sh_p_pos.setUniform1f("c_num", (float)c_num);
        
        //ofDrawRectangle(0, 0, particles_p_PingPong.src->getTexture().getWidth(), particles_p_PingPong.src->getTexture().getHeight());
        
        particles_p_PingPong.src->draw(0, 0);
        
        sh_p_pos.end();
    }
    particles_p_PingPong.dst->end();
    particles_p_PingPong.swap();
    
    
    fbo_render.begin();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    {
        sh_p_render.begin();
        
        sh_p_render.setUniformMatrix4f("projection", projection);
        sh_p_render.setUniformMatrix4f("view", view);
        sh_p_render.setUniformMatrix4f("model", model);
        
        sh_p_render.setUniform1f("_LineWidth",0.05);
        sh_p_render.setUniformTexture("pos_texture", particles_p_PingPong.dst->getTexture(), 1);
        sh_p_render.setUniform2f("res", (float)ofGetWidth(), (float)ofGetHeight());
        
        particles_mesh.draw();
        
        sh_p_render.end();
    }
    fbo_render.end();
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    
    fbo_render.draw(0,0,ofGetWidth(),ofGetHeight());
    
    ofSetColor(255);
    ofDrawBitmapString("fps: " + ofToString((int)ofGetFrameRate()), 20, 20);
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
