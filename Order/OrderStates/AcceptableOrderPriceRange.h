#ifndef HAR_ORDERPRICERANGE_H
#define HAR_ORDERPRICERANGE_H


class AcceptableOrderPriceRange {
public:
	explicit AcceptableOrderPriceRange(double lower = 0, double upper = 0);

	void setLowerPrice(double low);
	void setUpperPrice(double high);
	double lowerPrice() const;
	double upperPrice() const;

private:
	double m_lowerPrice;
	double m_upperPrice;
};

#endif //HAR_ORDERPRICERANGE_H
