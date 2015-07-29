/*
 * AnyData.h
 *
 *  Created on: Mar 11, 2014
 *      Author: linyisheng
 */

#ifndef ANYDATA_H_
#define ANYDATA_H_

class AnyData {

public:
	template<typename RealDataType>
	AnyData(RealDataType *data)	: m_RealData(data) {}

	template<typename RealDataType>
	RealDataType& GetRealDataType(){
		//D_CHECK(m_RealData);
		return *reinterpret_cast<RealDataType*>(m_RealData);
	}

	template<typename RealDataType>
	const RealDataType& GetRealDataType() const{
		//D_CHECK(m_RealData);
		return *reinterpret_cast<const RealDataType*>(m_RealData);
	}

	template<typename RealDataType>
	AnyData& operator =(RealDataType* _Data) {
		Set<RealDataType>(_Data);
		return *this;
	}

private:
	template<typename RealDataType>
	void Set(RealDataType* _Data) {
		m_RealData = reinterpret_cast<void*>(_Data);
	}

	private:
		void *m_RealData;
};

#endif /* ANYDATA_H_ */
