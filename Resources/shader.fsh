#ifdef GL_ES
precision mediump float;
#endif

varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform float x;
uniform float y;
uniform float width;
uniform float height;
uniform float frame;
float test = 0.0;
void main(void)
{
    float theAlpha = 0.2;
    
//    theAlpha = APx;
    if (v_texCoord.x>x &&v_texCoord.x<(x+frame))
    {
        theAlpha = (frame-(v_texCoord.x-x))/frame;
    }
    else if(v_texCoord.x>(height-x-frame) &&v_texCoord.x<(height-x))
    {
        theAlpha = (v_texCoord.x-x)/frame;
    }
    
    if (v_texCoord.y>y &&v_texCoord.y<(y+frame))
    {
        theAlpha = (frame-(v_texCoord.y-y))/frame;
    }
    else if(v_texCoord.y>(height-y-frame) &&v_texCoord.y<(height-y))
    {
        theAlpha = (v_texCoord.y-y) / frame;
    }
    if ((v_texCoord.y>y &&v_texCoord.y<(y+frame)) &&(v_texCoord.x>x &&v_texCoord.x<(x+frame)))
    {
        if (v_texCoord.y>v_texCoord.x)
        {
            theAlpha = (frame-(v_texCoord.y-y))/frame;
        }
        else
        {
            theAlpha = (frame-(v_texCoord.x-x))/frame;
        }
    }
    if((v_texCoord.x>x &&v_texCoord.x<(x+frame)) &&(v_texCoord.y>(height-y-frame) &&v_texCoord.y<(height-y)))
    {
        if (v_texCoord.y>v_texCoord.x)
        {
            theAlpha = (v_texCoord.y-y)/frame;
        }
        else
        {
            theAlpha = (frame-(v_texCoord.x-x))/frame;
        }
    }
    if ((v_texCoord.x>(height-x-frame) &&v_texCoord.x<(height-x)) && (v_texCoord.y>y &&v_texCoord.y<(y+frame)))
    {
        if(v_texCoord.y>v_texCoord.x)
        {
            theAlpha = (frame-(v_texCoord.y-y))/frame;
        }
        else
        {
            theAlpha = (v_texCoord.x-x)/frame;
        }
    }
    if ((v_texCoord.x>(height-x-frame) &&v_texCoord.x<(height-x))&&(v_texCoord.y>(height-y-frame) &&v_texCoord.y<(height-y)))
    {
        if(v_texCoord.y>v_texCoord.x)
        {
            theAlpha = (v_texCoord.y-y)/frame;
        }
        else
        {
            theAlpha = (v_texCoord.x-x)/frame;
        }
    }
//    theAlpha = 0.2;
    theAlpha=test;
    gl_FragColor = texture2D(CC_Texture0, v_texCoord)*theAlpha*v_fragmentColor;
    test = test+0.1;
}


