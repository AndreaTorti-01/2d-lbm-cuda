#include "experiment.h"
#include "shader.h"
#include "lbm.h"
#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <stdlib.h>
#include <math.h>


GLuint texture;
GLint program;
unsigned char *texture_buffer;


const float vertices[] = {
	-1.0f, -1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f
};


const float texture_vertices[] = {
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 0.0f,
};


GLchar vertex_shader_src[] = R"(
	attribute vec3 position;
	attribute vec2 atexCoord;

	varying vec2 texCoord;

	void main() {
		gl_Position = vec4(position, 1.0);
		texCoord = atexCoord;
	}
)";


GLchar fragment_shader_src[] = R"(
	precision mediump float;

	varying vec2 texCoord;
	uniform sampler2D tex;

	void main() {
		gl_FragColor = texture2D(tex, texCoord);
	}
)";


// suppose to know the u_out variable and the obstacles variable
void experiment_populate_texture() {
	// dummy texture generation
	for (int row = 0; row < height; ++row) {
		for (int col = 0; col < width; ++col) {
			unsigned char *base = texture_buffer + 3 * (row * width + col);

			if (obstacles[row * width + col]) {
				base[0] = 255;
				base[1] = 255;
				base[2] = 255;
			}
			else {
				// assuming u_out is in [0, 0.3]
				const float u = u_out[row * width + col];
				unsigned char color = (unsigned char) floor((u / 0.3f) * 255.0);

				base[0] = color;
				base[1] = color;
				base[2] = 20;
			}
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_buffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, texture);
}


// to be called after initializing lbm
void experiment_init(int width, int height) {
	program = program_load(vertex_shader_src, fragment_shader_src);

	glBindAttribLocation(program, 0, "position");
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);

	glBindAttribLocation(program, 1, "atexCoord");
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, texture_vertices);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	// RGB rendering so for each cell we need 3 colors
	texture_buffer = (unsigned char *) malloc(width * height * sizeof(unsigned char) * 3);
}


void experiment_resize(int width, int height) {
	glViewport(0, 0, width, height);
}


void experiment_render() {
	glClear(GL_COLOR_BUFFER_BIT);

	experiment_populate_texture();

	glUseProgram(program);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
