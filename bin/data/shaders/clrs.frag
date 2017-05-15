
precision mediump float;
uniform vec2 resolution;
uniform vec2 mouse;
uniform float time;

void main(){

    vec3 p = vec3((gl_FragCoord.xy-resolution/9.)/(resolution.y),mouse.x);
    for (float i = 0.; i < 9.; i++){
       p = abs((sin(p)/dot(p, p)-1.) * sin(time/100.) * 4. * mouse.x );
       if(length(p) > 1. && length(p) < 0.1) break;
    }


    gl_FragColor.rgb = p;
    gl_FragColor.a = 1.0;
}


