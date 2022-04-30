#include "SOM.h"
#include <glad/glad.h>
#include <iostream>
#include <stdlib.h>
#include <ctime>

SOM::SOM(int width, int height,int unitWidth,int unitHeight)
{
	srand(static_cast<unsigned>(time(0)));
	this->meshHeight = height;
	this->meshWidth = width;
	this->learningRate = .5;
	this->vertices = nullptr;
	this->meshVertices = nullptr;
	this->nodeArray = nullptr;
	this->widthSpacing = 1;
	this->heightSpacing = 1;
	this->xOffset = 3;

	if (width < 2)
	{
		this->meshWidth = 2;
	}

	if (height < 2)
	{
		this->meshHeight = 2;
	}

	

	
	

	


}

SOM::~SOM()
{
	delete[] vertices;
	delete[] meshVertices;
	delete[] nodeArray;
}

void SOM::InitializeSOMCrossConfiguration(void)
{
	if (this->nodeArray != nullptr)
	{
		delete[] nodeArray;
	}

	this->nodeArray = new Node[meshWidth * meshHeight];

	for (int i = 0; i < meshHeight; i++)
	{
		for (int j = 0; j < meshWidth; j++)
		{
			Node newNode;
			float randomX = -10 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (20)));
			float randomY = -10 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (20)));
			float randomZ = -10 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (20)));

			newNode.position = glm::vec3(randomX, randomY, randomZ);
			newNode.color = glm::vec3(1, 1, 1);

			newNode.up = -1;
			newNode.down = -1;
			newNode.left = -1;
			newNode.right = -1;
			newNode.upleft = -1;
			newNode.upright = -1;
			newNode.downleft = -1;
			newNode.downright = -1;

			if ((j - 1) >= 0)
			{
				int index = meshWidth * i + (j - 1);
				newNode.left = index;

			}
			

			if ((j + 1) < meshWidth)
			{
				int index = meshWidth * i + (j + 1);
				newNode.right = index;
			}
			

			if ((i - 1) >= 0)
			{
				int index = meshWidth * (i - 1) + j;
				newNode.up = index;
			}
			

			if ((i + 1) < meshHeight)
			{
				int index = meshWidth * (i + 1) + j;
				newNode.down = index;
			}
			

			



			int index = meshWidth * i + j;

			nodeArray[index] = newNode;

		}
	}

	this->numVertex = (this->meshWidth - 1) * (this->meshHeight - 1) * 6;

	if (vertices != nullptr)
	{
		delete[] vertices;
	}

	vertices = new float[(this->numVertex) * 3];
	int vertexCounter = 0;

	for (int i = 0; i < (this->meshHeight - 1); i++)
	{
		for (int j = 0; j < (this->meshWidth - 1); j++)
		{
			int NodeIndex = (this->meshWidth * i) + j;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.x;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.y;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.z;
			vertexCounter++;

			NodeIndex = (this->meshWidth * (i + 1)) + (j);

			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.x;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.y;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.z;
			vertexCounter++;

			NodeIndex = (this->meshWidth * i) + (j + 1);
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.x;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.y;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.z;
			vertexCounter++;

			NodeIndex = (this->meshWidth * (i + 1)) + (j);
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.x;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.y;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.z;
			vertexCounter++;

			NodeIndex = (this->meshWidth * (i + 1)) + (j + 1);
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.x;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.y;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.z;
			vertexCounter++;

			NodeIndex = (this->meshWidth * (i)) + (j + 1);

			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.x;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.y;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.z;
			vertexCounter++;




		}
	}

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);


	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->numVertex * 3, vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);


	if (meshVertices != nullptr)
	{
		delete[] meshVertices;
	}

	this->numVertex = (this->meshWidth) * (this->meshHeight);


	meshVertices = new float[(this->numVertex) * 6];
	vertexCounter = 0;

	for (int i = 0; i < (this->meshHeight); i++)
	{
		for (int j = 0; j < (this->meshWidth); j++)
		{
			int NodeIndex = (this->meshWidth * i) + j;
			meshVertices[vertexCounter] = j*widthSpacing+xOffset;
			vertexCounter++;
			meshVertices[vertexCounter] = (this->meshHeight-1-i)*heightSpacing;
			vertexCounter++;
			meshVertices[vertexCounter] = 0;
			vertexCounter++;
			meshVertices[vertexCounter] = this->nodeArray[NodeIndex].color.r;
			vertexCounter++;
			meshVertices[vertexCounter] = this->nodeArray[NodeIndex].color.g;
			vertexCounter++;
			meshVertices[vertexCounter] = this->nodeArray[NodeIndex].color.b;
			vertexCounter++;






		}
	}

	glDeleteBuffers(1, &meshVBO);
	glDeleteVertexArrays(1, &meshVAO);

	glGenVertexArrays(1, &meshVAO);
	glGenBuffers(1, &meshVBO);
	glBindVertexArray(meshVAO);

	glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->numVertex * 6, meshVertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


}

void SOM::InitializeSOMXConfiguration(void)
{
	if (this->nodeArray != nullptr)
	{
		delete[] nodeArray;
	}

	this->nodeArray = new Node[meshWidth * meshHeight];

	for (int i = 0; i < meshHeight; i++)
	{
		for (int j = 0; j < meshWidth; j++)
		{
			Node newNode;
			float randomX = -10 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (20)));
			float randomY = -10 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (20)));
			float randomZ = -10 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (20)));

			newNode.position = glm::vec3(randomX, randomY, randomZ);
			newNode.color = glm::vec3(1, 1, 1);

			newNode.up = -1;
			newNode.down = -1;
			newNode.left = -1;
			newNode.right = -1;
			newNode.upleft = -1;
			newNode.upright = -1;
			newNode.downleft = -1;
			newNode.downright = -1;

			

			

			if ((i - 1) >= 0 && (j - 1) >= 0)
			{
				int index = meshWidth * (i - 1) + (j - 1);
				newNode.upleft = index;
			}
			

			if ((j + 1) < meshWidth && (i - 1) >= 0)
			{
				int index = meshWidth * (i - 1) + (j + 1);
				newNode.upright = index;
			}
			

			if ((j - 1) >= 0 && (i + 1) < meshHeight)
			{
				int index = meshWidth * (i + 1) + (j - 1);
				newNode.downleft = index;
			}
			

			if ((j + 1) < meshWidth && (i + 1) < meshHeight)
			{
				int index = meshWidth * (i + 1) + (j + 1);
				newNode.downright = index;
			}
			



			int index = meshWidth * i + j;

			nodeArray[index] = newNode;

		}
	}

	this->numVertex = (this->meshWidth - 1) * (this->meshHeight - 1) * 6;

	if (vertices != nullptr)
	{
		delete[] vertices;
	}

	vertices = new float[(this->numVertex) * 3];
	int vertexCounter = 0;

	for (int i = 0; i < (this->meshHeight - 1); i++)
	{
		for (int j = 0; j < (this->meshWidth - 1); j++)
		{
			int NodeIndex = (this->meshWidth * i) + j;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.x;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.y;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.z;
			vertexCounter++;

			NodeIndex = (this->meshWidth * (i + 1)) + (j);

			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.x;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.y;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.z;
			vertexCounter++;

			NodeIndex = (this->meshWidth * i) + (j + 1);
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.x;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.y;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.z;
			vertexCounter++;

			NodeIndex = (this->meshWidth * (i + 1)) + (j);
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.x;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.y;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.z;
			vertexCounter++;

			NodeIndex = (this->meshWidth * (i + 1)) + (j + 1);
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.x;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.y;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.z;
			vertexCounter++;

			NodeIndex = (this->meshWidth * (i)) + (j + 1);

			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.x;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.y;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.z;
			vertexCounter++;




		}
	}

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->numVertex * 3, vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	if (meshVertices != nullptr)
	{
		delete[] meshVertices;
	}

	this->numVertex = (this->meshWidth) * (this->meshHeight);

	meshVertices = new float[(this->numVertex) * 6];
	vertexCounter = 0;

	for (int i = 0; i < (this->meshHeight); i++)
	{
		for (int j = 0; j < (this->meshWidth); j++)
		{
			int NodeIndex = (this->meshWidth * i) + j;
			meshVertices[vertexCounter] = j*widthSpacing+xOffset;
			vertexCounter++;
			meshVertices[vertexCounter] = (meshHeight-1-i)*heightSpacing;
			vertexCounter++;
			meshVertices[vertexCounter] = 0;
			vertexCounter++;
			meshVertices[vertexCounter] = this->nodeArray[NodeIndex].color.r;
			vertexCounter++;
			meshVertices[vertexCounter] = this->nodeArray[NodeIndex].color.g;
			vertexCounter++;
			meshVertices[vertexCounter] = this->nodeArray[NodeIndex].color.b;
			vertexCounter++;






		}
	}

	glDeleteBuffers(1, &meshVBO);
	glDeleteVertexArrays(1, &meshVAO);

	glGenVertexArrays(1, &meshVAO);
	glGenBuffers(1, &meshVBO);
	glBindVertexArray(meshVAO);

	glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->numVertex * 6, meshVertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);




}

void SOM::InitializeSOMStarConfiguration(void)
{
	if (nodeArray != nullptr)
	{
		delete[] nodeArray;
	}

	this->nodeArray = new Node[meshWidth * meshHeight];

	for (int i = 0; i < meshHeight; i++)
	{
		for (int j = 0; j < meshWidth; j++)
		{
			Node newNode;
			float randomX = -10 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (20)));
			float randomY = -10 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (20)));
			float randomZ = -10 + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (20)));

			newNode.position = glm::vec3(randomX, randomY, randomZ);
			newNode.color = glm::vec3(1, 1, 1);

			newNode.up = -1;
			newNode.down = -1;
			newNode.left = -1;
			newNode.right = -1;
			newNode.upleft = -1;
			newNode.upright = -1;
			newNode.downleft = -1;
			newNode.downright = -1;

			if ((j - 1) >= 0)
			{
				int index = meshWidth * i + (j - 1);
				newNode.left = index;

			}
			

			if ((j + 1) < meshWidth)
			{
				int index = meshWidth * i + (j + 1);
				newNode.right = index;
			}
			

			if ((i - 1) >= 0)
			{
				int index = meshWidth * (i - 1) + j;
				newNode.up = index;
			}
			

			if ((i + 1) < meshHeight)
			{
				int index = meshWidth * (i + 1) + j;
				newNode.down = index;
			}
			

			if ((i - 1) >= 0 && (j - 1) >= 0)
			{
				int index = meshWidth * (i - 1) + (j - 1);
				newNode.upleft = index;
			}
			

			if ((j + 1) < meshWidth && (i - 1) >= 0)
			{
				int index = meshWidth * (i - 1) + (j + 1);
				newNode.upright = index;
			}
			

			if ((j - 1) >= 0 && (i + 1) < meshHeight)
			{
				int index = meshWidth * (i + 1) + (j - 1);
				newNode.downleft = index;
			}
			

			if ((j + 1) < meshWidth && (i + 1) < meshHeight)
			{
				int index = meshWidth * (i + 1) + (j + 1);
				newNode.downright = index;
			}
			



			int index = meshWidth * i + j;

			nodeArray[index] = newNode;

		}
	}

	this->numVertex = (this->meshWidth - 1) * (this->meshHeight - 1) * 6;

	if (vertices != nullptr)
	{
		delete[] vertices;
	}

	vertices = new float[(this->numVertex) * 3];
	int vertexCounter = 0;

	for (int i = 0; i < (this->meshHeight - 1); i++)
	{
		for (int j = 0; j < (this->meshWidth - 1); j++)
		{
			int NodeIndex = (this->meshWidth * i) + j;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.x;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.y;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.z;
			vertexCounter++;

			NodeIndex = (this->meshWidth * (i + 1)) + (j);

			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.x;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.y;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.z;
			vertexCounter++;

			NodeIndex = (this->meshWidth * i) + (j + 1);
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.x;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.y;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.z;
			vertexCounter++;

			NodeIndex = (this->meshWidth * (i + 1)) + (j);
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.x;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.y;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.z;
			vertexCounter++;

			NodeIndex = (this->meshWidth * (i + 1)) + (j + 1);
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.x;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.y;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.z;
			vertexCounter++;

			NodeIndex = (this->meshWidth * (i)) + (j + 1);

			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.x;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.y;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.z;
			vertexCounter++;




		}
	}

	glDeleteBuffers(1, &VBO);
	glDeleteVertexArrays(1, &VAO);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->numVertex * 3, vertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	if (meshVertices != nullptr)
	{
		delete[] meshVertices;
	}

	this->numVertex = (this->meshWidth) * (this->meshHeight);

	meshVertices = new float[(this->numVertex) * 6];
	vertexCounter = 0;

	for (int i = 0; i < (this->meshHeight); i++)
	{
		for (int j = 0; j < (this->meshWidth); j++)
		{
			int NodeIndex = (this->meshWidth * i) + j;
			meshVertices[vertexCounter] = j*widthSpacing+xOffset;
			vertexCounter++;
			meshVertices[vertexCounter] = (meshHeight-1-i)*heightSpacing;
			vertexCounter++;
			meshVertices[vertexCounter] = 0;
			vertexCounter++;
			meshVertices[vertexCounter] = this->nodeArray[NodeIndex].color.r;
			vertexCounter++;
			meshVertices[vertexCounter] = this->nodeArray[NodeIndex].color.g;
			vertexCounter++;
			meshVertices[vertexCounter] = this->nodeArray[NodeIndex].color.b;
			vertexCounter++;






		}
	}

	glDeleteBuffers(1, &meshVBO);
	glDeleteVertexArrays(1, &meshVAO);

	glGenVertexArrays(1, &meshVAO);
	glGenBuffers(1, &meshVBO);
	glBindVertexArray(meshVAO);

	glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * this->numVertex * 6, meshVertices, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}

void SOM::Draw()
{
	
	UpdateVBO();
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, this->numVertex);

}

void SOM::DrawMesh()
{
	UpdateMeshVBO();
	glBindVertexArray(meshVAO);
	glDrawArrays(GL_POINTS, 0, this->numVertex);
}

void SOM::UpdateVBO()
{
	this->numVertex = (this->meshWidth - 1) * (this->meshHeight - 1) * 6;

	
	int vertexCounter = 0;
	

	for (int i = 0; i < (this->meshHeight - 1); i++)
	{
		for (int j = 0; j < (this->meshWidth - 1); j++)
		{
			int NodeIndex = (this->meshWidth * i) + j;
			vertices[vertexCounter] = (this->nodeArray[NodeIndex]).position.x;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.y;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.z;
			vertexCounter++;
			NodeIndex = (this->meshWidth * (i + 1)) + (j);
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.x;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.y;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.z;
			vertexCounter++;
			NodeIndex = (this->meshWidth * i) + (j + 1);
			
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.x;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.y;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.z;
			vertexCounter++;
			NodeIndex = (this->meshWidth * (i + 1)) + (j);
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.x;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.y;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.z;
			vertexCounter++;
			NodeIndex = (this->meshWidth * (i + 1)) + (j + 1);
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.x;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.y;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.z;
			vertexCounter++;
			NodeIndex = (this->meshWidth * (i)) + (j + 1);
			
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.x;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.y;
			vertexCounter++;
			vertices[vertexCounter] = this->nodeArray[NodeIndex].position.z;
			vertexCounter++;



		}
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*this->numVertex*3, vertices);
	glBindVertexArray(0);



	

}

void SOM::UpdateMeshVBO()
{
	this->numVertex = (this->meshWidth) * (this->meshHeight);

	meshVertices = new float[(this->numVertex) * 6];
	int vertexCounter = 0;

	for (int i = 0; i < (this->meshHeight); i++)
	{
		for (int j = 0; j < (this->meshWidth); j++)
		{
			int NodeIndex = (this->meshWidth * i) + j;
			meshVertices[vertexCounter] = j * widthSpacing+xOffset;
			vertexCounter++;
			meshVertices[vertexCounter] = (meshHeight - 1 - i) * heightSpacing;
			vertexCounter++;
			meshVertices[vertexCounter] = 0;
			vertexCounter++;
			meshVertices[vertexCounter] = this->nodeArray[NodeIndex].color.r;
			vertexCounter++;
			meshVertices[vertexCounter] = this->nodeArray[NodeIndex].color.g;
			vertexCounter++;
			meshVertices[vertexCounter] = this->nodeArray[NodeIndex].color.b;
			vertexCounter++;

			






		}
	}

	glBindVertexArray(meshVAO);
	glBindBuffer(GL_ARRAY_BUFFER, meshVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float)*this->numVertex * 6, meshVertices);
	glBindVertexArray(0);


}


void SOM::UpdateSOM(float* mesh,int numVertices,glm::mat4 transform)
{
	
	
	

	
		int randomIndex = rand() % (numVertices);
		float maximumValue = -1000000000;
		int maxIndex = -1;

		float originalX = mesh[randomIndex*6];
		float originalY = mesh[randomIndex*6 + 1];
		float originalZ = mesh[randomIndex*6 + 2];
		float red = mesh[randomIndex * 6 + 3];
		float green = mesh[randomIndex * 6 + 4];
		float blue = mesh[randomIndex * 6 + 5];

		glm::vec4 points = glm::vec4(originalX, originalY, originalZ, 1.0f);
		points = transform * points;

		originalX = points.x;
		originalY = points.y;
		originalZ = points.z;

		for (int j = 0; j < (meshWidth * meshHeight); j++)
		{
			float weightX = nodeArray[j].position.x;
			float weightY = nodeArray[j].position.y;
			float weightZ = nodeArray[j].position.z;

			float dotProduct = (weightX * originalX) + (weightY * originalY) + (weightZ * originalZ);
			if (dotProduct > maximumValue)
			{
				maxIndex = j;
				maximumValue = dotProduct;
			}



		}

		
		//Update neighbours and weights

		float deltaWx = learningRate * (originalX - nodeArray[maxIndex].position.x);
		float deltaWy = learningRate * (originalY - nodeArray[maxIndex].position.y);
		float deltaWz = learningRate * (originalZ - nodeArray[maxIndex].position.z);


		nodeArray[maxIndex].position.x += deltaWx;
		nodeArray[maxIndex].position.y += deltaWy;
		nodeArray[maxIndex].position.z += deltaWz;
		nodeArray[maxIndex].color.r = red;
		nodeArray[maxIndex].color.g = green;
		nodeArray[maxIndex].color.b = blue;




		if (nodeArray[maxIndex].up != -1)
		{
			float deltaWx = learningRate * (originalX - nodeArray[nodeArray[maxIndex].up].position.x);
			float deltaWy = learningRate * (originalY - nodeArray[nodeArray[maxIndex].up].position.y);
			float deltaWz = learningRate * (originalZ - nodeArray[nodeArray[maxIndex].up].position.z);

			nodeArray[nodeArray[maxIndex].up].position.x += deltaWx;
			nodeArray[nodeArray[maxIndex].up].position.y += deltaWy;
			nodeArray[nodeArray[maxIndex].up].position.z += deltaWz;
			nodeArray[nodeArray[maxIndex].up].color.r = red;
			nodeArray[nodeArray[maxIndex].up].color.g = green;
			nodeArray[nodeArray[maxIndex].up].color.b = blue;

		}
		
		

		if (nodeArray[maxIndex].down != -1)
		{
			float deltaWx = learningRate * (originalX - nodeArray[nodeArray[maxIndex].down].position.x);
			float deltaWy = learningRate * (originalY - nodeArray[nodeArray[maxIndex].down].position.y);
			float deltaWz = learningRate * (originalZ - nodeArray[nodeArray[maxIndex].down].position.z);

			nodeArray[nodeArray[maxIndex].down].position.x += deltaWx;
			nodeArray[nodeArray[maxIndex].down].position.y += deltaWy;
			nodeArray[nodeArray[maxIndex].down].position.z += deltaWz;
			nodeArray[nodeArray[maxIndex].down].color.r = red;
			nodeArray[nodeArray[maxIndex].down].color.g = green;
			nodeArray[nodeArray[maxIndex].down].color.b = blue;

		}
		

		if (nodeArray[maxIndex].left != -1)
		{
			float deltaWx = learningRate * (originalX - nodeArray[nodeArray[maxIndex].left].position.x);
			float deltaWy = learningRate * (originalY - nodeArray[nodeArray[maxIndex].left].position.y);
			float deltaWz = learningRate * (originalZ - nodeArray[nodeArray[maxIndex].left].position.z);

			nodeArray[nodeArray[maxIndex].left].position.x += deltaWx;
			nodeArray[nodeArray[maxIndex].left].position.y += deltaWy;
			nodeArray[nodeArray[maxIndex].left].position.z += deltaWz;
			nodeArray[nodeArray[maxIndex].left].color.r = red;
			nodeArray[nodeArray[maxIndex].left].color.g = green;
			nodeArray[nodeArray[maxIndex].left].color.b = blue;


		}
		

		if (nodeArray[maxIndex].right != -1)
		{
			float deltaWx = learningRate * (originalX - nodeArray[nodeArray[maxIndex].right].position.x);
			float deltaWy = learningRate * (originalY - nodeArray[nodeArray[maxIndex].right].position.y);
			float deltaWz = learningRate * (originalZ - nodeArray[nodeArray[maxIndex].right].position.z);

			nodeArray[nodeArray[maxIndex].right].position.x += deltaWx;
			nodeArray[nodeArray[maxIndex].right].position.y += deltaWy;
			nodeArray[nodeArray[maxIndex].right].position.z += deltaWz;
			nodeArray[nodeArray[maxIndex].right].color.r = red;
			nodeArray[nodeArray[maxIndex].right].color.g = green;
			nodeArray[nodeArray[maxIndex].right].color.b = blue;

		}
		


		if (nodeArray[maxIndex].upleft != -1)
		{
			float deltaWx = learningRate * (originalX - nodeArray[nodeArray[maxIndex].upleft].position.x);
			float deltaWy = learningRate * (originalY - nodeArray[nodeArray[maxIndex].upleft].position.y);
			float deltaWz = learningRate * (originalZ - nodeArray[nodeArray[maxIndex].upleft].position.z);

			nodeArray[nodeArray[maxIndex].upleft].position.x += deltaWx;
			nodeArray[nodeArray[maxIndex].upleft].position.y += deltaWy;
			nodeArray[nodeArray[maxIndex].upleft].position.z += deltaWz;
			nodeArray[nodeArray[maxIndex].upleft].color.r = red;
			nodeArray[nodeArray[maxIndex].upleft].color.g = green;
			nodeArray[nodeArray[maxIndex].upleft].color.b = blue;

		}
		

		if (nodeArray[maxIndex].upright != -1)
		{
			float deltaWx = learningRate * (originalX - nodeArray[nodeArray[maxIndex].upright].position.x);
			float deltaWy = learningRate * (originalY - nodeArray[nodeArray[maxIndex].upright].position.y);
			float deltaWz = learningRate * (originalZ - nodeArray[nodeArray[maxIndex].upright].position.z);

			nodeArray[nodeArray[maxIndex].upright].position.x += deltaWx;
			nodeArray[nodeArray[maxIndex].upright].position.y += deltaWy;
			nodeArray[nodeArray[maxIndex].upright].position.z += deltaWz;
			nodeArray[nodeArray[maxIndex].upright].color.r = red;
			nodeArray[nodeArray[maxIndex].upright].color.g = green;
			nodeArray[nodeArray[maxIndex].upright].color.b = blue;

		}
		

		if (nodeArray[maxIndex].downright != -1)
		{
			float deltaWx = learningRate * (originalX - nodeArray[nodeArray[maxIndex].downright].position.x);
			float deltaWy = learningRate * (originalY - nodeArray[nodeArray[maxIndex].downright].position.y);
			float deltaWz = learningRate * (originalZ - nodeArray[nodeArray[maxIndex].downright].position.z);

			nodeArray[nodeArray[maxIndex].downright].position.x += deltaWx;
			nodeArray[nodeArray[maxIndex].downright].position.y += deltaWy;
			nodeArray[nodeArray[maxIndex].downright].position.z += deltaWz;
			nodeArray[nodeArray[maxIndex].downright].color.r = red;
			nodeArray[nodeArray[maxIndex].downright].color.g = green;
			nodeArray[nodeArray[maxIndex].downright].color.b = blue;

		}
		

		if (nodeArray[maxIndex].downleft != -1)
		{
			float deltaWx = learningRate * (originalX - nodeArray[nodeArray[maxIndex].downleft].position.x);
			float deltaWy = learningRate * (originalY - nodeArray[nodeArray[maxIndex].downleft].position.y);
			float deltaWz = learningRate * (originalZ - nodeArray[nodeArray[maxIndex].downleft].position.z);

			nodeArray[nodeArray[maxIndex].downleft].position.x += deltaWx;
			nodeArray[nodeArray[maxIndex].downleft].position.y += deltaWy;
			nodeArray[nodeArray[maxIndex].downleft].position.z += deltaWz;
			nodeArray[nodeArray[maxIndex].downleft].color.r = red;
			nodeArray[nodeArray[maxIndex].downleft].color.g = green;
			nodeArray[nodeArray[maxIndex].downleft].color.b = blue;

		}
		

		



	
	
	


}


void SOM::SetMeshWidth(int width)
{
	this->meshWidth = width;
}

void SOM::SetMeshHeight(int height)
{
	this->meshHeight = height;
}

void SOM::SetLearningRate(float lr)
{
	this->learningRate = lr;
}
