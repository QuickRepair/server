#include "ContactInformation.h"

ContactInformation::ContactInformation(std::string address)
	: m_address{std::move(address)}
{}
