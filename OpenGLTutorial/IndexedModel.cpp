#include "IndexedModel.hpp"

#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>

static bool CompareOBJIndexPtr(const OBJIndex* a, const OBJIndex* b);
static inline size_t FindNextChar(size_t start, const char* str, size_t length, char token);
static inline unsigned int ParseOBJIndexValue(const std::string& token, size_t start, size_t end);
static inline float ParseOBJFloatValue(const std::string& token, size_t start, size_t end);
static inline std::vector<std::string> SplitString(const std::string &s, char delim);

OBJModel::OBJModel(const std::string& fileName)
{
	hasUVs = false;
	hasNormals = false;
    std::ifstream file;
    file.open(fileName.c_str());

    std::string line;
    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
        
            size_t lineLength = line.length();
            
            if(lineLength < 2)
                continue;
            
            const char* lineCStr = line.c_str();
            
            switch(lineCStr[0])
            {
                case 'v':
                    if(lineCStr[1] == 't')
                        this->uvs.push_back(ParseOBJVec2(line));
                    else if(lineCStr[1] == 'n')
                        this->normals.push_back(ParseOBJVec3(line));
                    else if(lineCStr[1] == ' ' || lineCStr[1] == '\t')
                        this->vertices.push_back(ParseOBJVec3(line));
                break;
                case 'f':
                    CreateOBJFace(line);
                break;
                default: break;
            };
        }
    }
    else
    {
        std::cerr << "Unable to load mesh: " << fileName << std::endl;
    }
}

void IndexedModel::CalcNormals()
{
    for(size_t i = 0; i < indices.size(); i += 3)
    {
        size_t i0 = indices[i    ];
        size_t i1 = indices[i + 1];
        size_t i2 = indices[i + 2];

        glm::vec3 v1 = positions[i1] - positions[i0];
        glm::vec3 v2 = positions[i2] - positions[i0];
        
        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
            
        normals[i0] -= normal;
        normals[i1] -= normal;
        normals[i2] -= normal;
    }
    
    for(size_t i = 0; i < positions.size(); i++)
        normals[i] = glm::normalize(normals[i]);
}

void IndexedModel::CalcTangents()
{
	for(size_t i = 0; i < indices.size(); i += 3)
	{
        size_t i0 = indices[i    ];
        size_t i1 = indices[i + 1];
        size_t i2 = indices[i + 2];

		glm::vec3 edge1 = positions[i1] - positions[i0];
		glm::vec3 edge2 = positions[i2] - positions[i0];

		glm::vec2 delta1 = texCoords[i1] - texCoords[i0];
		glm::vec2 delta2 = texCoords[i2] - texCoords[i0];

		float dividend = (delta1.x * delta2.y - delta2.x * delta1.y);
		float f = 1.0f / dividend;

		glm::vec3 tangent(0, 0, 0);
		tangent.x = f * (delta2.y * edge1.x - delta1.y * edge2.x);
		tangent.y = f * (delta2.y * edge1.y - delta1.y * edge2.y);
		tangent.z = f * (delta2.y * edge1.z - delta1.y * edge2.z);

		tangents[i0] += tangent;
        tangents[i1] += tangent;
        tangents[i2] += tangent;
	}
}

IndexedModel OBJModel::ToIndexedModel()
{
    IndexedModel result;
    IndexedModel normalModel;
    
    size_t numIndices = OBJIndices.size();
    
    std::vector<OBJIndex*> indexLookup;
    
    for(size_t i = 0; i < numIndices; i++)
        indexLookup.push_back(&OBJIndices[i]);
    
    std::sort(indexLookup.begin(), indexLookup.end(), CompareOBJIndexPtr);
    
    std::map<OBJIndex, size_t> normalModelIndexMap;
    std::map<size_t, size_t> indexMap;
    
    for(unsigned int i = 0; i < numIndices; i++)
    {
        OBJIndex* currentIndex = &OBJIndices[i];
        
        glm::vec3 currentPosition = vertices[currentIndex->vertexIndex];
        glm::vec2 currentTexCoord;
        glm::vec3 currentNormal;
        
        if(hasUVs)
            currentTexCoord = uvs[currentIndex->uvIndex];
        else
            currentTexCoord = glm::vec2(0, 0);
            
        if(hasNormals)
            currentNormal = normals[currentIndex->normalIndex];
        else
            currentNormal = glm::vec3(0, 0, 0);
        
        uint32_t normalModelIndex;
        uint32_t resultModelIndex;
        
        //Create model to properly generate normals on
        std::map<OBJIndex, size_t>::iterator it = normalModelIndexMap.find(*currentIndex);
        if(it == normalModelIndexMap.end())
        {
            normalModelIndex = uint32_t(normalModel.positions.size());
        
            normalModelIndexMap.insert(std::pair<OBJIndex, unsigned int>(*currentIndex, normalModelIndex));
            normalModel.positions.push_back(currentPosition);
            normalModel.texCoords.push_back(currentTexCoord);
            normalModel.normals.push_back(currentNormal);
            normalModel.tangents.push_back(glm::vec3());
        }
        else
            normalModelIndex = it->second;
        
        //Create model which properly separates texture coordinates
        uint32_t previousVertexLocation = FindLastVertexIndex(indexLookup, currentIndex, result);
        
        if(previousVertexLocation == uint32_t(-1))
        {
            resultModelIndex = uint32_t(result.positions.size());
        
            result.positions.push_back(currentPosition);
            result.texCoords.push_back(currentTexCoord);
            result.normals.push_back(currentNormal);
            result.tangents.push_back(glm::vec3());
        }
        else
            resultModelIndex = previousVertexLocation;
        
        normalModel.indices.push_back(normalModelIndex);
        result.indices.push_back(resultModelIndex);
        indexMap.insert(std::pair<size_t, size_t>(resultModelIndex, normalModelIndex));
    }
    
    if(!hasNormals)
    {
        normalModel.CalcNormals();
        
        for(size_t i = 0; i < result.positions.size(); i++)
            result.normals[i] = normalModel.normals[indexMap[i]];
    }
    
    normalModel.CalcTangents();

    for(size_t i = 0; i < result.positions.size(); i++)
        result.tangents[i] = normalModel.tangents[indexMap[i]];

    return result;
};

unsigned int OBJModel::FindLastVertexIndex(const std::vector<OBJIndex*>& indexLookup, const OBJIndex* currentIndex, const IndexedModel& result)
{
   uint32_t start = 0;
   uint32_t end = uint32_t(indexLookup.size());
   uint32_t current = (end - start) / 2 + start;
   uint32_t previous = start;
    
    while(current != previous)
    {
        OBJIndex* testIndex = indexLookup[current];
        
        if(testIndex->vertexIndex == currentIndex->vertexIndex)
        {
            uint32_t countStart = current;
        
            for(size_t i = 0; i < current; i++)
            {
                OBJIndex* possibleIndex = indexLookup[current - i];
                
                if(possibleIndex == currentIndex)
                    continue;
                    
                if(possibleIndex->vertexIndex != currentIndex->vertexIndex)
                    break;
                    
                countStart--;
            }
            
            for(size_t i = countStart; i < indexLookup.size() - countStart; i++)
            {
                OBJIndex* possibleIndex = indexLookup[current + i];
                
                if(possibleIndex == currentIndex)
                    continue;
                    
                if(possibleIndex->vertexIndex != currentIndex->vertexIndex)
                    break;
                else if((!hasUVs || possibleIndex->uvIndex == currentIndex->uvIndex) 
                    && (!hasNormals || possibleIndex->normalIndex == currentIndex->normalIndex))
                {
                    glm::vec3 currentPosition = vertices[currentIndex->vertexIndex];
                    glm::vec2 currentTexCoord;
                    glm::vec3 currentNormal;
                    
                    if(hasUVs)
                        currentTexCoord = uvs[currentIndex->uvIndex];
                    else
                        currentTexCoord = glm::vec2(0, 0);
                        
                    if(hasNormals)
                        currentNormal = normals[currentIndex->normalIndex];
                    else
                        currentNormal = glm::vec3(0, 0, 0);
                    
                    for(size_t j = 0; j < result.positions.size(); j++)
                    {
                        if(currentPosition == result.positions[j] 
                            && ((!hasUVs || currentTexCoord == result.texCoords[j])
                            && (!hasNormals || currentNormal == result.normals[j])))
                        {
                            return j;
                        }
                    }
                }
            }
        
            return -1;
        }
        else
        {
            if(testIndex->vertexIndex < currentIndex->vertexIndex)
                start = current;
            else
                end = current;
        }
    
        previous = current;
        current = (end - start) / 2 + start;
    }
    
    return -1;
}

void OBJModel::CreateOBJFace(const std::string& line)
{
    std::vector<std::string> tokens = SplitString(line, ' ');

    this->OBJIndices.push_back(ParseOBJIndex(tokens[1], &this->hasUVs, &this->hasNormals));
    this->OBJIndices.push_back(ParseOBJIndex(tokens[2], &this->hasUVs, &this->hasNormals));
    this->OBJIndices.push_back(ParseOBJIndex(tokens[3], &this->hasUVs, &this->hasNormals));

    if((int)tokens.size() > 4)
    {
        this->OBJIndices.push_back(ParseOBJIndex(tokens[1], &this->hasUVs, &this->hasNormals));
        this->OBJIndices.push_back(ParseOBJIndex(tokens[3], &this->hasUVs, &this->hasNormals));
        this->OBJIndices.push_back(ParseOBJIndex(tokens[4], &this->hasUVs, &this->hasNormals));
    }
}

OBJIndex OBJModel::ParseOBJIndex(const std::string& token, bool* hasUVs, bool* hasNormals)
{
    size_t tokenLength = token.length();
    const char* tokenString = token.c_str();
    
    size_t vertIndexStart = 0;
    size_t vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');
    
    OBJIndex result;
    result.vertexIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
    result.uvIndex = 0;
    result.normalIndex = 0;
    
    if(vertIndexEnd >= tokenLength)
        return result;
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');
    
    result.uvIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
    *hasUVs = true;
    
    if(vertIndexEnd >= tokenLength)
        return result;
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');
    
    result.normalIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
    *hasNormals = true;
    
    return result;
}

glm::vec3 OBJModel::ParseOBJVec3(const std::string& line) 
{
    size_t tokenLength = line.length();
    const char* tokenString = line.c_str();
    
    size_t vertIndexStart = 2;
    
    while(vertIndexStart < tokenLength)
    {
        if(tokenString[vertIndexStart] != ' ')
            break;

        vertIndexStart++;
    }
    
    size_t vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float z = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    return glm::vec3(x, y, -z);
}

glm::vec2 OBJModel::ParseOBJVec2(const std::string& line)
{
    size_t tokenLength = line.length();
    const char* tokenString = line.c_str();
    
    unsigned int vertIndexStart = 3;
    
    while(vertIndexStart < tokenLength)
    {
        if(tokenString[vertIndexStart] != ' ')
            break;
        vertIndexStart++;
    }
    
    unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    return glm::vec2(x,y);
}

static bool CompareOBJIndexPtr(const OBJIndex* a, const OBJIndex* b)
{
    return a->vertexIndex < b->vertexIndex;
}

static inline size_t FindNextChar(size_t start, const char* str, size_t length, char token)
{
    size_t result = start;
    while(result < length)
    {
        result++;
        if(str[result] == token)
            break;
    }
    
    return result;
}

static inline uint32_t ParseOBJIndexValue(const std::string& token, size_t start, size_t end)
{
    return atoi(token.substr(start, end - start).c_str()) - 1;
}

static inline float ParseOBJFloatValue(const std::string& token, size_t start, size_t end)
{
    return float(atof(token.substr(start, end - start).c_str()));
}

static inline std::vector<std::string> SplitString(const std::string &s, char delim)
{
    std::vector<std::string> elems;
        
    const char* cstr = s.c_str();
    size_t strLength = s.length();
    size_t start = 0;
    size_t end = 0;
        
    while(end <= strLength)
    {
        while(end <= strLength)
        {
            if(cstr[end] == delim)
                break;
            end++;
        }
            
        elems.push_back(s.substr(start, end - start));
        start = end + 1;
        end = start;
    }
        
    return elems;
}