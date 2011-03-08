
#include "Shader.h"

Shader::Shader(GLchar *fs, GLchar *vs) {
	char *vsf = NULL;
	char *fsf = NULL;
	vsf = textFileRead(vs); // Load the vertex shader
	fsf = textFileRead(fs); // Load the fragment shader
	const char *ff = fsf;
	const char *vv = vsf;

	if (vsf != NULL && fsf != NULL) { // If the files were loaded successfully, create the shader objects
		v = glCreateShader(GL_VERTEX_SHADER);
		f = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(v, 1, &vv, NULL);
		glShaderSource(f, 1, &ff, NULL);
		glCompileShader(v); // Compile the shader files
		glCompileShader(f);
		p = glCreateProgram();
		glAttachShader(p, v); // Put everything together
		glAttachShader(p, f);
		glLinkProgram(p);
		glUseProgram(p);

		int infologLength = 0;
		int charsWritten  = 0;
		char *infoLog;

		glGetProgramiv(p, GL_INFO_LOG_LENGTH, &infologLength); // Print any errors reported by the shader during compilation 
		if (infologLength > 0) {
			infoLog = (char *)malloc(infologLength);
			glGetProgramInfoLog(p, infologLength, &charsWritten, infoLog);
			printf("%s\n",infoLog);
			free(infoLog);
		}
	}
	if (vsf == NULL) { // Errors if the shader files were not found
		cout << "Cannot find shader file " << vs << endl;
	}
	if (fsf == NULL) {
		cout << "Cannot find shader file " << fs << endl;
	}
}

void Shader::on() {
	glUseProgram(p);
}

void Shader::off() {
	glUseProgram(NULL);
}

// Code below modified from code originally written by Wojtek Palubicki
char *Shader::textFileRead(const char *fn) {
	FILE *fp;
	char *content = NULL;

	int count = 0;

	if (fn != NULL) {

		fp = fopen(fn,"r");
		
		if (fp != NULL) {

			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);

			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

int Shader::textFileWrite(char *fn, char *s) {

	FILE *fp;
	int status = 0;

	if (fn != NULL) {
		fp = fopen(fn,"w");

		if (fp != NULL) {

			if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
				status = 1;
			fclose(fp);
		}
	}
	return(status);
}

// Returns the location of the given shader uniform, or an error if it was not found
int Shader::getUniLoc(const char *name) {
	int loc;
	loc = glGetUniformLocation(p, name);

	if (loc == -1)
		printf("No such uniform named \"%s\"\n", name);

	return loc;
}


// Returns the location of the given shader attribute, or an error if it was not found
int Shader::getAttrLoc(const char *name) {
	int loc;
	loc = glGetAttribLocation(p, name);

	if (loc == -1)
		printf("No such attribute named \"%s\"\n", name);

	return loc;
}
