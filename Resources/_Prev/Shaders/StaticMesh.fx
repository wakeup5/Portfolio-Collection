
//
// ��������
//

float4x4 matWorld : World;
float4x4 matViewProjection : ViewProjection;
float4 vEyePos : ViewPosition;

float camNear;			//ī�޶� �ٰŸ� ���
float camFar;			//ī�޶� ���Ÿ� ���



texture Diffuse_Tex;
sampler2D Diffuse = sampler_state
{
   Texture = (Diffuse_Tex);
   MAGFILTER = LINEAR;
   MIPFILTER = LINEAR;
   MINFILTER = LINEAR;
};
texture Normal_Tex;
sampler2D Normal = sampler_state
{
   Texture = (Normal_Tex);
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};

texture Specular_Tex;
sampler2D Specular = sampler_state
{
   Texture = (Specular_Tex);
   MAGFILTER = LINEAR;
   MINFILTER = LINEAR;
   MIPFILTER = LINEAR;
};


texture Emission_Tex;
sampler2D Emission = sampler_state
{
   Texture = (Emission_Tex);
   MAGFILTER = LINEAR;
   MIPFILTER = LINEAR;
   MINFILTER = LINEAR;
};

float fSpecPower;

// ����Ʈ ���
// Pos.x,         Pos.y,          Pos.z,         lightType ( 0 Direction, 1 Point, 2 Spot )
// Dir.x,         Dir.y,          Dir.z,         DistancePow;
// Color.r,       Color.g,        Color.b,       strength
// MaxRange,      MinRange,       InAngle        OutAngle 
float4x4 matLights[10];
int LightNum = 0;

float4x4 baseDirectionLight;			//�⺻������ ���





//---------------------------------------------------------------
// Base ����
//---------------------------------------------------------------

struct VS_INPUT 
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   float3 Normal : NORMAL0;
   float3 Binormal : BINORMAL0;
   float3 Tangent : TANGENT0;
};

struct VS_OUTPUT 
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   float3 Normal : TEXCOORD1;
   float3 Binormal : TEXCOORD2;
   float3 Tangent : TEXCOORD3;
   float3 viewDir : TEXCOORD4;
   float3 worldPos : TEXCOORD5;
};

VS_OUTPUT vs_main( VS_INPUT Input )
{
   VS_OUTPUT Output = (VS_OUTPUT)0;

   float4 worldPos = mul( Input.Position, matWorld );
   Output.Position = mul( worldPos, matViewProjection );
   
   Output.Texcoord = Input.Texcoord;
   
   Output.Normal = mul( Input.Normal, (float3x3)matWorld );
   Output.Binormal = mul( Input.Binormal, (float3x3)matWorld );  
   Output.Tangent = mul( Input.Tangent, (float3x3)matWorld ); 
   
   Output.viewDir = vEyePos.xyz - worldPos.xyz;
   Output.worldPos = worldPos;

   return( Output );
}




struct PS_INPUT
{
   float2 Texcoord : TEXCOORD0;
   float3 Normal : TEXCOORD1;
   float3 Binormal : TEXCOORD2;
   float3 Tangent : TEXCOORD3;
   float3 viewDir : TEXCOORD4;
   float3 worldPos : TEXCOORD5;
};




void ComputeLight( 
		out float3 addDiffuse,			//���Ǽ� ������ DeffuseColor
		out float3 addSpecular,			//���Ǽ� ������ SpecularColor
		float3 position,				//��ġ ��
		float3 normal,					//�븻
		float3 viewDir,					//�ü� ����
		int lightIndex					//ó���Ǵ� ����Ʈ �ε���
		)
{
	float4x4 matLight = matLights[ lightIndex ];

	float3 lightColor = float3( matLight._31, matLight._32, matLight._33 ) * matLight._34;

	//Direction ó��
	if( matLight._14 == 0.0f )
	{
		//������ ����
		float3 dir = float3( matLight._21, matLight._22, matLight._23 );
		float3 lightDir = -dir;

		float NdotL = dot( lightDir, normal );
		float diff = saturate( NdotL );

		//����Ʈ �ݻ�
		float3 lightRefl = normalize( dir + 2.0f * NdotL * normal );
		float spec = saturate( dot( lightRefl, viewDir ) );
		spec = pow( spec, fSpecPower );
		
		addDiffuse = lightColor * diff;
		addSpecular =  lightColor * spec * diff;
	}

	//Point Light and Spot ó��
	else
	{
		float3 lightPosition = float3( matLight._11, matLight._12, matLight._13 );
		float maxRange = matLight._41;
		float minRange = matLight._42;
		float DistancePow = matLight._24;

		//������ ����
		float3 lightDir = lightPosition - position;

		//������ �Ÿ�
		float dist = length( lightDir );
		
		//����ȭ
		lightDir *= ( 1.0f / dist );

		//���� ��....
		float NdotL = dot( lightDir, normal );
		float diff = saturate( NdotL );

		//�Ÿ��� ���� ������
		float t = saturate( ( dist - minRange ) / ( maxRange - minRange ) );
		t = 1.0f - pow( t, DistancePow );
		
		//���� Spot �̶��....

		

		diff *= t;			//�޴� ���� ���� ������ ����

		//����Ʈ �ݻ�
		float3 lightRefl = normalize( -lightDir + 2.0f * NdotL * normal );
		float spec = saturate( dot( lightRefl, viewDir ) );
		spec = pow( spec, fSpecPower );
		
		addDiffuse = lightColor * diff;
		addSpecular =  lightColor * spec * diff;		//Spec �� Diff �� ������ �޴´�.


	}

	


}


float4 ps_main( PS_INPUT Input ) : COLOR0
{   
     //TBN Matrix
     float3x3 TBN = float3x3(
        normalize( Input.Tangent ),
        normalize( Input.Binormal ),
        normalize( Input.Normal ) );
     
	 //   
     // WorldNormal
     //
     float3 norColor = tex2D( Normal, Input.Texcoord ).rgb;
     
	 //Tangent Space Normal
     float3 spaceNor = ( norColor * 2.0f ) - 1.0f;

     float3 worldNormal = mul( spaceNor, TBN );
     float3 viewDir = normalize( Input.viewDir );
  
	 //���� ��
	 float3 finalDiffuse = float3( 0, 0, 0 );
	 float3 finalSpecular = float3( 0, 0, 0 );

	 //�⺻ ������ ó��

	 //������ ����
	 float3 dir = float3( baseDirectionLight._21, baseDirectionLight._22, baseDirectionLight._23 );
 	 float3 lightDir = -dir;
	 
	 //������ �÷�
	 float3 lightColor = float3( baseDirectionLight._31, baseDirectionLight._32, baseDirectionLight._33 ) * baseDirectionLight._34;

	 
	 float NdotL = dot( lightDir, worldNormal );
	 
	 //Ambient
	 //float diff = saturate( NdotL );
	 
	 //float diff = max( 0.1f, NdotL );
	 
	 float diff = NdotL;
	 if( diff < 0.0 )
		diff = abs( NdotL ) * 0.3f;


	 //����Ʈ �ݻ�
	 float3 lightRefl = normalize( dir + 2.0f * NdotL * worldNormal );
	 float spec = saturate( dot( lightRefl, viewDir ) );
	 spec = pow( spec, fSpecPower );
	
	 //�⺻ ���� ����
	 finalDiffuse = lightColor * diff;
	 finalSpecular =  lightColor * spec * diff;		//Specular �� diff �Ȱ��ϸ� Specular �� ���� ������ ���´�...




	 //�߰��Ǵ� ����
	 float3 addDiffuse = float3( 0, 0, 0 );
	 float3 addSpecular = float3( 0, 0, 0 );
	 for( int i = 0 ; i < LightNum ; i++ )
	 {
		//i �ε����� ������ ���Ǿ� addDiffuse, addSpecular �� ���Եȴ�.
		ComputeLight( 
			addDiffuse, 
			addSpecular,
			Input.worldPos, 
			worldNormal,
			viewDir,
			i );

		//���� ���� ���� ���δ�.
		finalDiffuse += addDiffuse;
		finalSpecular += addSpecular;
	 }


     //
     // Diffuse
     //
     float3 diffuseColor = tex2D( Diffuse, Input.Texcoord ).rgb * finalDiffuse;
     
     //
     // Specular 
     //
     float3 specularColor = tex2D( Specular, Input.Texcoord ).rgb * finalSpecular;
     
     //
     // Emission
     //
     float3 emissionColor = tex2D( Emission, Input.Texcoord ).rgb;
		
     //
     // Final Color 
     //
     float3 finalColor = diffuseColor + specularColor + emissionColor;
      
     return float4( finalColor, 1 );
   
}



//���� ���� PixelShader
float4 ps_mainTree( PS_INPUT Input ) : COLOR0
{   
	float4 diffTex = tex2D( Diffuse, Input.Texcoord );
	clip( diffTex.a - 0.1f );


     //TBN Matrix
     float3x3 TBN = float3x3(
        normalize( Input.Tangent ),
        normalize( Input.Binormal ),
        normalize( Input.Normal ) );
     
	 //   
     // WorldNormal
     //
     float3 norColor = tex2D( Normal, Input.Texcoord ).rgb;
     
	 //Tangent Space Normal
     float3 spaceNor = ( norColor * 2.0f ) - 1.0f;

     float3 worldNormal = mul( spaceNor, TBN );
     float3 viewDir = normalize( Input.viewDir );
  
	 //���� ��
	 float3 finalDiffuse = float3( 0, 0, 0 );
	 float3 finalSpecular = float3( 0, 0, 0 );

	 //�⺻ ������ ó��

	 //������ ����
	 float3 dir = float3( baseDirectionLight._21, baseDirectionLight._22, baseDirectionLight._23 );
 	 float3 lightDir = -dir;
	 
	 //������ �÷�
	 float3 lightColor = float3( baseDirectionLight._31, baseDirectionLight._32, baseDirectionLight._33 ) * baseDirectionLight._34;

	 
	 float NdotL = dot( lightDir, worldNormal );
	 
	 //Ambient
	 //float diff = saturate( NdotL );
	 
	 float diff = max( 0.3f, NdotL );
	 
	 //float diff = NdotL;
	 //if( diff < 0.0 )
	 //	diff = abs( NdotL ) * 0.1f;


	 //����Ʈ �ݻ�
	 float3 lightRefl = normalize( dir + 2.0f * NdotL * worldNormal );
	 float spec = saturate( dot( lightRefl, viewDir ) );
	 spec = pow( spec, fSpecPower );
	
	 //�⺻ ���� ����
	 finalDiffuse = lightColor * diff;
	 finalSpecular =  lightColor * spec * diff;		//Specular �� diff �Ȱ��ϸ� Specular �� ���� ������ ���´�...




	 //�߰��Ǵ� ����
	 float3 addDiffuse = float3( 0, 0, 0 );
	 float3 addSpecular = float3( 0, 0, 0 );
	 for( int i = 0 ; i < LightNum ; i++ )
	 {
		//i �ε����� ������ ���Ǿ� addDiffuse, addSpecular �� ���Եȴ�.
		ComputeLight( 
			addDiffuse, 
			addSpecular,
			Input.worldPos, 
			worldNormal,
			viewDir,
			i );

		//���� ���� ���� ���δ�.
		finalDiffuse += addDiffuse;
		finalSpecular += addSpecular;
	 }


     //
     // Diffuse
     //
	 
     float3 diffuseColor = diffTex.rgb * finalDiffuse;
     
     //
     // Specular 
     //
     float3 specularColor = tex2D( Specular, Input.Texcoord ).rgb * finalSpecular;
     
     //
     // Emission
     //
     float3 emissionColor = tex2D( Emission, Input.Texcoord ).rgb;
		
     //
     // Final Color 
     //
     float3 finalColor = diffuseColor + specularColor + emissionColor;
      

	  
     return float4( finalColor, 1 );
   
}



//---------------------------------------------------------------
// Depth ����
//---------------------------------------------------------------

struct VS_INPUT_DEPTH
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
};

struct VS_OUTPUT_DEPTH
{
   float4 Position : POSITION0;
   float4 FinalPos : TEXCOORD0;
   float2 Texcoord : TEXCOORD1;

};


VS_OUTPUT_DEPTH vs_depth( VS_INPUT_DEPTH Input )
{
   VS_OUTPUT_DEPTH Output = (VS_OUTPUT_DEPTH)0;

   float4 worldPos = mul( Input.Position, matWorld );
   Output.Position = mul( worldPos, matViewProjection );
   
   Output.FinalPos = Output.Position;

   Output.Texcoord = Input.Texcoord;

   return( Output );
}


float4 ps_depth( VS_OUTPUT_DEPTH Input ) : COLOR0
{   
	//��ĺ�ȯ�� ��ģ �� z �� ��ĺ�ȯ���� ��� ����ġ w �� ������ 0 ~ 1 ������ ���� ���� �ȴ�.
	float depth = Input.FinalPos.z / Input.FinalPos.w;

	//���� depth ���� ī�޶��� near �� far �� �̿��Ͽ� �������� ���ش�....
	//Perspective Projection Linear Depth
    float z = depth;
    float a = camFar / ( camFar - camNear );
    float b = -camNear / ( camFar - camNear );
    depth = b / ( z - a ); 


    return float4( depth, depth, depth, 1 );
}



//---------------------------------------------------------------
// CreateShadow ����
//---------------------------------------------------------------

float4 ps_CreateShadow( VS_OUTPUT_DEPTH Input ) : COLOR0
{   
	//��ĺ�ȯ�� ��ģ �� z �� ��ĺ�ȯ���� ��� ����ġ w �� ������ 0 ~ 1 ������ ���� ���� �ȴ�.
	float depth = Input.FinalPos.z / Input.FinalPos.w;


	float4 diffTex = tex2D( Diffuse, Input.Texcoord );
	clip( diffTex.a - 0.1f );

    //return float4( depth, depth, depth, 1 );

	return float4( depth.xxx, 1 );
}


//--------------------------------------------------------------//
// Render With ShadowMap 
//--------------------------------------------------------------//


float4x4 matLightViewProjection;			//���⼺ ���� ViewProjection ���

//������ Texture
texture Shadow_Tex;
sampler2D ShadowSampler = sampler_state
{
   Texture = (Shadow_Tex);
   ADDRESSU = BORDER;
   ADDRESSV = BORDER;
   BORDERCOLOR = 0xFFFFFFFF;
};

float bias = 0.01f;


struct VS_INPUT_RECIVESHADOW
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   float3 Normal : NORMAL0;
   float3 Binormal : BINORMAL0;
   float3 Tangent : TANGENT0;
};

struct VS_OUTPUT_RECIVESHADOW
{
   float4 Position : POSITION0;
   float2 Texcoord : TEXCOORD0;
   float3 Normal : TEXCOORD1;
   float3 Binormal : TEXCOORD2;
   float3 Tangent : TEXCOORD3;
   float3 viewDir : TEXCOORD4;
   float3 worldPos : TEXCOORD5;

   float4 FinalPos : TEXCOORD6;
   float4 LightClipPos : TEXCOORD7;		//���� ���忡�� �ٶ� ��ġ
};


VS_OUTPUT_RECIVESHADOW vs_ReciveShadow( VS_INPUT_RECIVESHADOW Input )
{
   VS_OUTPUT_RECIVESHADOW Output = (VS_OUTPUT_RECIVESHADOW)0;

   float4 worldPos = mul( Input.Position, matWorld );
   Output.Position = mul( worldPos, matViewProjection );
   
   Output.Texcoord = Input.Texcoord;
   
   Output.Normal = mul( Input.Normal, (float3x3)matWorld );
   Output.Binormal = mul( Input.Binormal, (float3x3)matWorld );  
   Output.Tangent = mul( Input.Tangent, (float3x3)matWorld ); 
   
   Output.viewDir = vEyePos.xyz - worldPos.xyz;
   Output.worldPos = worldPos;


   Output.FinalPos = Output.Position;		//��ȯ ����
   Output.LightClipPos = mul( worldPos, matLightViewProjection );	//���� ���忡�� �� ��ġ

   return( Output );
}


float4 ps_ReciveShadow( VS_OUTPUT_RECIVESHADOW Input ) : COLOR0
{  
	//���� ���忡�� �ٶ� ��ġ�� ���� �� ( ����Ʈ ����� �����̱⶧���� �������� ���ɴ� )
	float lightDepth = Input.LightClipPos.z / Input.LightClipPos.w;

	//Shadow ���� UV ����
	//( -1 ~ 1 )
	float2 shadowUV = Input.LightClipPos.xy / Input.LightClipPos.w;
	shadowUV.y = -shadowUV.y;		//y����...
	//0 ~ 1 �� ����
	shadowUV = ( shadowUV * 0.5f ) + 0.5f;	

	//���� �׷����� ���� ��ġ�� DirectionLight ���忡�� ���� ���̰�...
	float shadowDepth = tex2D( ShadowSampler, shadowUV ).r;

	//�׸��ڰ� �׷����� ��Ȳ�� shadowDepth + bias �� ���� lightDepth �� ū����̴�.




	//TBN Matrix
    float3x3 TBN = float3x3(
       normalize( Input.Tangent ),
       normalize( Input.Binormal ),
       normalize( Input.Normal ) );
     
	 //   
     // WorldNormal
     //
     float3 norColor = tex2D( Normal, Input.Texcoord ).rgb;
     
	 //Tangent Space Normal
     float3 spaceNor = ( norColor * 2.0f ) - 1.0f;

     float3 worldNormal = mul( spaceNor, TBN );
	 worldNormal = normalize( worldNormal );
     float3 viewDir = normalize( Input.viewDir );
  
	 //���� ��
	 float3 finalDiffuse = float3( 0, 0, 0 );
	 float3 finalSpecular = float3( 0, 0, 0 );

	 //�⺻ ������ ó��

	 //������ ����
	 float3 dir = float3( baseDirectionLight._21, baseDirectionLight._22, baseDirectionLight._23 );
 	 float3 lightDir = -dir;
	 
	 //������ �÷�
	 float3 lightColor = float3( baseDirectionLight._31, baseDirectionLight._32, baseDirectionLight._33 ) * baseDirectionLight._34;

	 
	 float NdotL = dot( lightDir, worldNormal );
	 
	 //Ambient
	 //float diff = saturate( NdotL );
	 
	 //float diff = max( 0.1f, NdotL );
	 
	 float diff = NdotL;
	 
	 //�׸��ڰ� �׷����� �Ѵٸ� diff �� ������ ������ �ȴ� 
	 if( shadowDepth + bias < lightDepth )
		diff = abs( diff ) * -1.0f;

	 if( diff < 0.0 )
		diff = abs( diff ) * 0.3f;

	 //����Ʈ �ݻ�
	 float3 lightRefl = normalize( dir + 2.0f * NdotL * worldNormal );
	 float spec = saturate( dot( lightRefl, viewDir ) );
	 spec = pow( spec, fSpecPower );
	
	 //�⺻ ���� ����
	 finalDiffuse = lightColor * diff;
	 finalSpecular =  lightColor * spec * diff;		//Specular �� diff �Ȱ��ϸ� Specular �� ���� ������ ���´�...


	 //�߰��Ǵ� ����
	 float3 addDiffuse = float3( 0, 0, 0 );
	 float3 addSpecular = float3( 0, 0, 0 );
	 for( int i = 0 ; i < LightNum ; i++ )
	 {
		//i �ε����� ������ ���Ǿ� addDiffuse, addSpecular �� ���Եȴ�.
		ComputeLight( 
			addDiffuse, 
			addSpecular,
			Input.worldPos, 
			worldNormal,
			viewDir,
			i );

		//���� ���� ���� ���δ�.
		finalDiffuse += addDiffuse;
		finalSpecular += addSpecular;
	 }


     //
     // Diffuse
     //
     float3 diffuseColor = tex2D( Diffuse, Input.Texcoord ).rgb * finalDiffuse;
     
     //
     // Specular 
     //
     float3 specularColor = tex2D( Specular, Input.Texcoord ).rgb * finalSpecular;
     
     //
     // Emission
     //
     float3 emissionColor = tex2D( Emission, Input.Texcoord ).rgb;
		
     //
     // Final Color 
     //
     float3 finalColor = diffuseColor + specularColor + emissionColor;
      
     return float4( finalColor, 1 );


}







//--------------------------------------------------------------//
// Technique Section for Mesh
//--------------------------------------------------------------//
technique Base
{
   pass Pass_0
   {
      VertexShader = compile vs_3_0 vs_main();
      PixelShader = compile ps_3_0 ps_main();
   }
}

technique BaseTree
{
   pass Pass_0
   {
	  CULLMODE = NONE;
      VertexShader = compile vs_3_0 vs_main();
      PixelShader = compile ps_3_0 ps_mainTree();
   }
}


technique Depth
{
   pass Pass_0
   {
      VertexShader = compile vs_3_0 vs_depth();
      PixelShader = compile ps_3_0 ps_depth();
   }
}


technique CreateShadow
{
   pass Pass_0
   {
      VertexShader = compile vs_3_0 vs_depth();
      PixelShader = compile ps_3_0 ps_CreateShadow();
   }
}





technique ReciveShadow
{
   pass Pass_0
   {
      VertexShader = compile vs_3_0 vs_ReciveShadow();
      PixelShader = compile ps_3_0 ps_ReciveShadow();
   }
}