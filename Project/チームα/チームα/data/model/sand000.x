xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 26;
 -10.00118;0.07576;9.65656;,
 6.50927;0.46230;3.13657;,
 6.50945;1.30544;2.97802;,
 -10.00063;2.53925;9.19332;,
 6.50963;2.08266;2.61478;,
 -10.00012;4.81014;8.13199;,
 6.50978;2.74514;2.06965;,
 -9.99968;6.74574;6.53925;,
 6.50990;3.25123;1.37691;,
 -9.99932;8.22444;4.51519;,
 6.50998;3.56914;0.58007;,
 -9.99908;9.15332;2.18698;,
 6.51002;3.67890;-0.27080;,
 -9.99897;9.47401;-0.29909;,
 6.51001;3.57361;-1.12223;,
 -9.99900;9.16637;-2.78680;,
 6.50996;3.25988;-1.92073;,
 -9.99916;8.24973;-5.11985;,
 6.50986;2.75743;-2.61612;,
 -9.99945;6.78168;-7.15164;,
 6.50972;2.09783;-3.16471;,
 -9.99984;4.85446;-8.75451;,
 6.50956;1.32253;-3.53203;,
 -10.00032;2.58917;-9.82775;,
 6.50938;0.48024;-3.69499;,
 -10.00084;0.12816;-10.30391;;
 
 12;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,8,9;,
 4;9,8,10,11;,
 4;11,10,12,13;,
 4;13,12,14,15;,
 4;15,14,16,17;,
 4;17,16,18,19;,
 4;19,18,20,21;,
 4;21,20,22,23;,
 4;23,22,24,25;;
 
 MeshMaterialList {
  1;
  12;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "sand001.png";
   }
  }
 }
 MeshNormals {
  13;
  0.362505;0.176963;-0.915027;,
  0.358300;0.172455;0.917540;,
  0.356224;0.286547;0.889379;,
  0.347648;0.500427;0.792915;,
  0.340426;0.683799;0.645391;,
  0.335054;0.824488;0.456024;,
  0.331908;0.913062;0.236970;,
  0.331209;0.943554;0.002494;,
  0.333008;0.913926;-0.232046;,
  0.337176;0.826221;-0.451299;,
  0.343421;0.686399;-0.641029;,
  0.351310;0.503864;-0.789115;,
  0.360306;0.290736;-0.886371;;
  12;
  4;1,1,2,2;,
  4;2,2,3,3;,
  4;3,3,4,4;,
  4;4,4,5,5;,
  4;5,5,6,6;,
  4;6,6,7,7;,
  4;7,7,8,8;,
  4;8,8,9,9;,
  4;9,9,10,10;,
  4;10,10,11,11;,
  4;11,11,12,12;,
  4;12,12,0,0;;
 }
 MeshTextureCoords {
  26;
  0.520000;1.000000;,
  0.520000;0.500000;,
  0.560000;0.500000;,
  0.560000;1.000000;,
  0.600000;0.500000;,
  0.600000;1.000000;,
  0.640000;0.500000;,
  0.640000;1.000000;,
  0.680000;0.500000;,
  0.680000;1.000000;,
  0.720000;0.500000;,
  0.720000;1.000000;,
  0.760000;0.500000;,
  0.760000;1.000000;,
  0.800000;0.500000;,
  0.800000;1.000000;,
  0.840000;0.500000;,
  0.840000;1.000000;,
  0.880000;0.500000;,
  0.880000;1.000000;,
  0.920000;0.500000;,
  0.920000;1.000000;,
  0.960000;0.500000;,
  0.960000;1.000000;,
  1.000000;0.500000;,
  1.000000;1.000000;;
 }
}
