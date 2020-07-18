#version 330

out vec4 FragColor;

uniform sampler2DRect pos_texture;
uniform vec2 pos_res;
uniform float c_num;

in vec2 v_texcoord;

void main()
{
    vec2 tc = gl_FragCoord.xy - 0.5;
    vec4 pos = texture(pos_texture, tc);
    
    /*
    float steps = 1. / c_num;
    
    float ease_speed = 0.0004;
    
    float id = mod( tc.x, 4. );
    
    if( id == 0. ){
        if(tc.x > 1.){
            vec4 tpos = texture(pos_texture, tc + vec2(-2., 0.));
            pos = tpos;
        }else if(tc.y > 0.){
            vec4 tpos = texture(pos_texture, vec2(pos_res.x - 2., tc.y - 1.));
            pos = tpos;
        }
    }else if( id == 1. ){
        if(tc.x > 2.){
            vec4 tpos = texture(pos_texture, tc + vec2(-3., 0.));
            pos = tpos;
        }else if(tc.y > 0.){
            vec4 tpos = texture(pos_texture, vec2(pos_res.x - 2., tc.y - 1.));
            pos = tpos;
        }
    }else if( id == 2. ){
        vec4 tpos = texture(pos_texture, tc + vec2(-1., 0.));
        pos += (tpos - pos) * ease_speed;
    }else if( id == 3. ){
        if(tc.x < pos_res.x){
            vec4 tpos = texture(pos_texture, tc + vec2(2., 0.));
            pos = tpos;
        }else if(tc.y < pos_res.y - 1.){
            vec4 tpos = texture(pos_texture, vec2(1., tc.y + 1.));
            pos = tpos;
        }
    }
    */
    
    FragColor = clamp(pos,0.,1.);
}

/*
 #version 120
 
 //out vec4 FragColor;
 
 uniform sampler2DRect pos_texture;
 
 void main()
 {
 vec2 st = gl_FragCoord.xy;
 vec3 pos = texture2DRect(pos_texture, st).xyz;
 
 gl_FragColor = vec4(pos, 1.0);
 }
 */
