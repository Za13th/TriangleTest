struct PS_INPUT
{
    float4 position : SV_POSITION;
    float3 normal: NORMAL;
    float3 color : COLOR;
    float3 color1 : COLOR1;
};

cbuffer constant: register(b0)
{
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_proj;
    row_major float4x4 m_proj_inv;
    float m_angle;
};

cbuffer DirectionalLightBuffer : register(b1)
{
    float4 cLightDirection;
}


float4 psmain(PS_INPUT input) : SV_Target
{
        input.normal = normalize(input.normal);


        //float4 diffuse_light_colour = float4(0.8f, 0.8f, 0.8f, 1.0f);
        //float4 ambient_light_colour = float4(0.2f, 0.2f, 0.2f, 1.0f);
        //float4 specular_light_colour = float4(0.4f, 0.4f, 0.4f, 1.0f);

        float4 diffuse_light_colour = float4(0, 1.f, 0, 1.0f); //diffuse is green (yellow when mixed with specular)
        float4 ambient_light_colour = float4(1, 0.f, 0.f, 1.0f); //ambient is red
        float4 specular_light_colour = float4(0, 0, 1.f, 1.0f); // specular is blue

        float intensity = 1.0f;

        float3 light_direction = cLightDirection.xyz;

	    // Diffuse lighting
        float diffuse_factor = saturate(dot(-light_direction, input.normal));
        float4 diffuse_light = diffuse_factor * diffuse_light_colour;

        // Ambient lighting
        float4 ambient_light = ambient_light_colour;

	    // Specular lighting
        float4 specular_light = 0.0f;

            [flatten]
        if (diffuse_factor > 0.0f)
        {
            float3 view_direction = normalize(float3(0.0f,0.0f,-4.0f) - input.position);
            float3 reflect_direction = reflect(light_direction, input.normal);

            float specular_factor = pow(max(dot(view_direction, reflect_direction), 0.0), 16.f * intensity);
            specular_light = float4(specular_factor * specular_light_colour);
        }


        //float4 finalColor = (ambient_light) * intensity;
        //float4 finalColor = (diffuse_light) * intensity;
        float4 finalColor = (specular_light) * intensity;
        //float4 finalColor = (diffuse_light + ambient_light) * intensity;
        //float4 finalColor = (diffuse_light + ambient_light + specular_light) * intensity;

        return finalColor * float4(input.color,1.0f);
        //return float4(input.color,1.0f);

        //return float4(lerp(input.color, input.color1, (sin(m_angle) + 1.0f) / 2.0f),1.0f);
}