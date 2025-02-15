struct VSInput
{
    float4 position : POSITION;
    float4 color : COLOR;
};

struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

PSInput VS(VSInput input)
{
    PSInput output;

    output.position = input.position;
    output.color = input.color;

    return output;
}

float4 PS(PSInput input) : SV_TARGET
{
    return input.color;
}
