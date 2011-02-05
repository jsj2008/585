
#include "Shader.h"

Shader::Shader(GLchar *fs, GLchar *vs) {
	char *vsf = NULL;
	char *fsf = NULL;
	vsf = textFileRead(vs);
	fsf = textFileRead(fs);
	const char *ff = fsf;
	const char *vv = vsf;

	if (vsf != NULL && fsf != NULL) {
		v = glCreateShader(GL_VERTEX_SHADER);
		f = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(v, 1, &vv, NULL);
		glShaderSource(f, 1, &ff, NULL);
		glCompileShader(v);
		glCompileShader(f);
		p = glCreateProgram();
		glAttachShader(p, v);
		glAttachShader(p, f);
		glLinkProgram(p);
		glUseProgram(p);

		int infologLength = 0;
		int charsWritten  = 0;
		char *infoLog;

		glGetProgramiv(p, GL_INFO_LOG_LENGTH,&infologLength);
		if (infologLength > 0) {
			infoLog = (char *)malloc(infologLength);
			glGetProgramInfoLog(p, infologLength, &charsWritten, infoLog);
			printf("%s\n",infoLog);
			free(infoLog);
		}
	}
	if (vsf == NULL) {
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

/*
* Function: getUniLoc
* Purpose: gets the location of a uniform variable in a shader program
* Paramaters: program -> shader program
name -> the name of the variable to get
return -> the location of the variable

*/
int Shader::getUniLoc(const char *name) {
	int loc;
	loc = glGetUniformLocation(p, name);

	if (loc == -1)
		printf("No such uniform named \"%s\"\n", name);

	return loc;
}

/*
* Function: getUniLoc
* Purpose: gets the location of an attribute variable in a shader program
* Paramaters: program -> shader program
name -> the name of the variable to get
return -> the location of the variable
if location = -1 there was an error

*/
int Shader::getAttrLoc(const char *name) {
	int loc;
	loc = glGetAttribLocation(p, name);

	if (loc == -1)
		printf("No such uniform named \"%s\"\n", name);

	return loc;
}
