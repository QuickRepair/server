#ifndef HAR_ORDERPRICERANGE_H
#define HAR_ORDERPRICERANGE_H


class OrderPriceRange {
public:
	OrderPriceRange(double low = 0, double high = 0);

	void setPriceLow(double low);
	void setPriceHigh(double high);
	double priceLow() const;
	double priceHigh() const;

private:
	double m_priceLow;
	double m_priceHigh;
};


#endif //HAR_ORDERPRICERANGE_H
