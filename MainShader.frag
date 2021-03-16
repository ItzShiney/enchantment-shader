uniform sampler2D texture;
uniform float ticks;
uniform float size;

const float pi = acos(-1);

const vec4 purple = vec4(240/255.0,
                         190/255.0,
                         255/255.0,
                         1) * 1.3;

float blinkSize = 12;
const float blinkIntensity = 2.5;

float sqr(float x)
{ return x * x; }

float cot(float x)
{ return 1/tan(x); }

float min(float a, float b, float c, float d, float e)
{
    float res = a;
    if (b < res)
        res = b;
    if (c < res)
        res = c;
    if (d < res)
        res = d;
    if (e < res)
        return e;
    return res;
}

struct Blink
{
    float angle;
    float h;
};

float dist(float x, float y, float angle, float h)
{ return abs(cos(angle) * (y - (h - x*tan(angle)))); }

void main()
{
    float xNormal = gl_TexCoord[0].x;
    float yNormal = gl_TexCoord[0].y;
    float x = xNormal * size;
    float y = yNormal * size;

    float dy = min(dist(x, y, pi/8,      (40 + 2*blinkSize + size) * ticks               - 25),
                   dist(x, y, pi/8,      (40 + 2*blinkSize + size) * mod(ticks + 0.3, 1) - 25),
                   dist(x, y, pi - pi/4, (60 + 2*blinkSize + size) * ticks               - 40),
                   dist(x, y, pi - pi/4, (60 + 2*blinkSize + size) * mod(ticks + 0.5, 1) - 40),
                   dist(x, y, pi - pi/4, (60 + 2*blinkSize + size) * mod(ticks + 0.8, 1) - 40));

    float blinkMul = sqr((sin((max(0, (blinkSize - dy)/blinkSize) - 0.5) * pi/2) + 1)/2);
    float mul = 1 + blinkIntensity * blinkMul;

    vec4 pixel = texture2D(texture, vec2(xNormal, yNormal));
    gl_FragColor = gl_Color * pixel * mul * purple;
}
