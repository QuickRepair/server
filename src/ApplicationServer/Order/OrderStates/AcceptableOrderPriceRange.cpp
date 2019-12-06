#include "AcceptableOrderPriceRange.h"

AcceptableOrderPriceRange::AcceptableOrderPriceRange(double low, double high)
	: m_lowerPrice{low}, m_upperPrice{high}
{}

void AcceptableOrderPriceRange::setLowerPrice(double low)
{
	m_lowerPrice = low;
}

void AcceptableOrderPriceRange::setUpperPrice(double high)
{
	m_upperPrice = high;
}

double AcceptableOrderPriceRange::lowerPrice() const
{
	return m_lowerPrice;
}

double AcceptableOrderPriceRange::upperPrice() const
{
	return m_upperPrice;
}