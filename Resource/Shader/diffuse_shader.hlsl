// global
matrix WorldMatrix;
matrix ViewMatrix;
matrix ProjMatrix;

vector AmbientMtrl;
vector DiffuseMtrl;

vector LightDirection;

vector DiffuseLightIntensity = { 1.f, 1.f, 1.f, 1.f };
vector AmbientLightIntensity = { 0.2f, 0.2f, 0.2f, 1.f };

// struct
struct VS_INPUT
{
    vector position : POSITION;
    vector normal : NORMAL;
};

struct VS_OUTPUT
{
    vector position : POSITION;
    vector diffuse : COLOR;
};

// main function
VS_OUTPUT Main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    
    // to clip space
    output.position = mul(input.position, WorldMatrix);
    output.position = mul(output.position, ViewMatrix);
    output.position = mul(output.position, ProjMatrix);
    
    input.normal = mul(input.normal, WorldMatrix);
    float s = dot(LightDirection.xyz, input.normal.xyz);
    
    if (s < 0.f)
        s = 0.f;
    
    output.diffuse = (AmbientMtrl * AmbientLightIntensity) + 
                     (s * (DiffuseLightIntensity * DiffuseMtrl));

    return output;
}