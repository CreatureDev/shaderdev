#ifndef __GLFUNCS_H__
#define __GLFUNCS_H__

int glGetUniformLocation(unsigned int, const char *);
void glUniform1f(int, float);
void glUniform1i(int, int);
void glUniform2f(int, float, float);
void glUniform3f(int, float, float, float);
void glUniform4f(int, float, float, float, float);
void glUniformMatrix2fv(int, unsigned int, GLboolean, const void *);
void glUniformMatrix3fv(int, unsigned int, GLboolean, const void *);
void glUniformMatrix4fv(int, unsigned int, GLboolean, const void *);
void glGenBuffers(unsigned int, unsigned int *);
void glBindBuffer(GLenum, unsigned int);
void glBufferData(GLenum, unsigned int, const void *, GLenum);
int glGetAttribLocation(unsigned int, const char *);
void glEnableVertexAttribArray(unsigned int);
void glBufferSubData(GLenum, unsigned int, unsigned int, const void *);
void glVertexAttribPointer(unsigned int, int, GLenum, GLboolean, unsigned int, const void *);
void glDeleteBuffers(unsigned int, unsigned int *);


#endif /* __GLFUNCS_H__ */

