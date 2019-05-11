#include "ContactInformation.h"

ContactInformation::ContactInformation(std::string address, std::string telephone)
	: m_address{std::move(address)}, m_tel{std::move(telephone)}
{}
