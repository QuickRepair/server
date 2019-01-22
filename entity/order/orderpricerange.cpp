#include "orderpricerange.h"

OrderPriceRange::OrderPriceRange(double low, double high)
	: m_priceLow{low}, m_priceHigh{high}
{}

void OrderPriceRange::setPriceLow(double low)
{
	m_priceLow = low;
}

void OrderPriceRange::setPriceHigh(double high)
{
	m_priceHigh = high;
}

double OrderPriceRange::priceLow() const
{
	return m_priceLow;
}

double OrderPriceRange::priceHigh() const
{
	return m_priceHigh;
}