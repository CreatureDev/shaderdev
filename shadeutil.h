#ifndef __SHADEUTIL_H__
#define __SHADEUTIL_H__

void getshaderdat(const char *dsc);
void bindshaderdat(unsigned int gid, void *rnd);
void updateshader(unsigned int gid);
void rendershader();

#endif /* __SHADEUTIL_H__ */

