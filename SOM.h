#ifndef SOM_H
#define SOM_H
#include <glm/glm.hpp>

class SOM
{
public:
	SOM(int width, int height, int unitWidth = 1,int unitHeight=1);
	~SOM();
	void Draw();
	void DrawMesh();
	void UpdateSOM(float* mesh,int numVertices,glm::mat4 transform);
	void InitializeSOMCrossConfiguration(void);
	void InitializeSOMXConfiguration(void);
	void InitializeSOMStarConfiguration(void);
	void SetMeshWidth(int width);
	void SetMeshHeight(int height);
	void SetLearningRate(float lr);
	





private:

	struct Node
	{
		glm::vec3 position;
		glm::vec3 color;
		int up;
		int down;
		int left;
		int right;
		int upleft;
		int upright;
		int downleft;
		int downright;

	};

	int meshWidth;
	int meshHeight;
	Node* nodeArray;
	unsigned int VAO;
	unsigned int VBO;
	unsigned int meshVAO;
	unsigned int meshVBO;
	unsigned int numVertex;
	float* vertices;
	float* meshVertices;
	float learningRate;
	int widthSpacing;
	int heightSpacing;
	int xOffset;


private:
	void UpdateVBO();
	void UpdateMeshVBO();



};


#endif
