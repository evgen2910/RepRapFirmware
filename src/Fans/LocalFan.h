/*
 * LocalFan.h
 *
 *  Created on: 3 Sep 2019
 *      Author: David
 */

#ifndef SRC_FANS_LOCALFAN_H_
#define SRC_FANS_LOCALFAN_H_

#include "Fan.h"

class LocalFan : public Fan
{
public:
	LocalFan(unsigned int fanNum);
	~LocalFan();

	bool Check() override;								// update the fan PWM returning true if it is a thermostatic fan that is on
	bool IsEnabled() const override { return port.IsValid(); }
	GCodeResult SetPwmFrequency(PwmFrequency freq, const StringRef& reply) override;
	int32_t GetRPM() override;
	GCodeResult ReportPortDetails(const StringRef& str) const override;
#if SUPPORT_CAN_EXPANSION
	void UpdateRpmFromRemote(CanAddress src, int32_t rpm) override { }
#endif

	bool AssignPorts(const char *pinNames, const StringRef& reply);

	void Interrupt();

protected:
	GCodeResult Refresh(const StringRef& reply) override;
	bool UpdateFanConfiguration(const StringRef& reply) override;

private:
	void SetHardwarePwm(float pwmVal);
	void InternalRefresh();

	PwmPort port;											// port used to control the fan
	IoPort tachoPort;										// port used to read the tacho

	float lastPwm;

	// Variables used to read the tacho
	static constexpr uint32_t fanMaxInterruptCount = 32;	// number of fan interrupts that we average over
	uint32_t fanInterruptCount;								// accessed only in ISR, so no need to declare it volatile
	volatile uint32_t fanLastResetTime;						// time (in step clocks) at which we last reset the interrupt count, accessed inside and outside ISR
	volatile uint32_t fanInterval;							// written by ISR, read outside the ISR

	uint32_t blipStartTime;
	bool blipping;
};

#endif /* SRC_FANS_LOCALFAN_H_ */
