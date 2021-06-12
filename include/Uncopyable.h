#pragma once
class Uncopyable
{
private:
	Uncopyable(const Uncopyable&) {}
	Uncopyable& operator=(const Uncopyable&) {}
public:
	Uncopyable() = default;
};

