#ifndef SHADER_H
#define SHADER_H

#include <string>

class Shader
{
public:
	Shader(const char* vertex, const char* frag);
	~Shader();
	
	void use();

	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;

	unsigned int id;
};

#endif // SHADER_H
