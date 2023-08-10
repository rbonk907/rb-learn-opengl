#ifndef SHADER_H
#define SHADER_H

#include <glad/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Shader_T* Shader_T;

Shader_T Shader_new(const char* vertexPath, const char* fragmentPath);
void Shader_use(Shader_T sh);
void Shader_setBool(Shader_T sh, const char* name, bool value);
void Shader_setInt(Shader_T sh, const char* name, int value);
void Shader_setFloat(Shader_T sh, const char* name, float value);
/* utility functions */
static void getFileLength(FILE* file, long* length);
static void checkCompileErrors(unsigned int shaderID, const char* type);

struct Shader_T {
  unsigned int ID;
};

Shader_T Shader_new(const char* vertexPath, const char* fragmentPath) {
  char* vertexCode = NULL;
  char* fragmentCode = NULL;
  long length = 0;
  
  Shader_T sh = (Shader_T)malloc(sizeof(struct Shader_T));
  if (sh == NULL) {
    printf("Memory not allocated.\n");
    exit(EXIT_FAILURE);
  }
  /* retrieve vertex and fragment source code from filePath */
  /* ------------------------------------------------------ */
  FILE* vShaderFile = fopen(vertexPath, "rb");
  FILE* fShaderFile = fopen(fragmentPath, "rb");
  if (!vShaderFile || !fShaderFile)
  {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  getFileLength(vShaderFile, &length);
  vertexCode = (char*)malloc(length + 1);
  /* copy contents of file into vertexCode */
  if (vertexCode)
  {
    if (fread(vertexCode, 1, length, vShaderFile) != length)
    {
      perror("fread");
      exit(EXIT_FAILURE);
    }
    vertexCode[length] = '\0';
  }

  if (fclose(vShaderFile))
  {
    perror("fclose");
    exit(EXIT_FAILURE);
  }

  getFileLength(fShaderFile, &length);
  fragmentCode = (char*)malloc(length + 1);
  /* copy contents of file into fragmentCode */
  if (fragmentCode)
  {
    if (fread(fragmentCode, 1, length, fShaderFile) != length)
    {
      perror("fread");
      exit(EXIT_FAILURE);
    }
    fragmentCode[length] = '\0';
  }

  if (fclose(fShaderFile))
  {
    perror("fclose");
    exit(EXIT_FAILURE);
  }
  /* compile the shaders */
  /* ------------------- */
  const char* vShaderCode = vertexCode;
  const char* fShaderCode = fragmentCode;
  unsigned int vertex, fragment;

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);
  checkCompileErrors(vertex, "VERTEX");

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);
  checkCompileErrors(fragment, "FRAGMENT");

  sh->ID = glCreateProgram();
  glAttachShader(sh->ID, vertex);
  glAttachShader(sh->ID, fragment);
  glLinkProgram(sh->ID);
  checkCompileErrors(sh->ID, "PROGRAM");
  
  // clean up
  glDeleteShader(vertex);
  glDeleteShader(fragment);
  free(vertexCode);
  free(fragmentCode);

  return sh;
}

void Shader_free(Shader_T sh) {
  free(sh);
}

void Shader_use(Shader_T sh) {
  glUseProgram(sh->ID);
}

void Shader_setBool(Shader_T sh, const char* name, bool value)
{
  glUniform1i(glGetUniformLocation(sh->ID, name), (int)value);
}

void Shader_setInt(Shader_T sh, const char* name, int value)
{
  glUniform1i(glGetUniformLocation(sh->ID, name), value);
}

void Shader_setFloat(Shader_T sh, const char* name, float value)
{
  glUniform1f(glGetUniformLocation(sh->ID, name), value);
}

/* utility functions */
/* --------------------------------------------------------------- */
void getFileLength(FILE* file, long* length)
{
  fseek(file, 0, SEEK_END);
  *length = ftell(file);
  rewind(file);
}

void checkCompileErrors(unsigned int shader, const char* type)
{
  int success;
  char infoLog[1024];
  if (type != "PROGRAM")
  {
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(shader, 1024, NULL, infoLog);
      printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n", type);
      printf("%s\n", infoLog);
    }
  } 
  else
  {
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(shader, 1024, NULL, infoLog);
      printf("ERROR::PROGRAM_LINKING_ERROR of type: %s\n", type);
      printf("%s\n", infoLog);
    }
  } 
}

#endif