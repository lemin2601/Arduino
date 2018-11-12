#include<Arduino.h>
#include "Timer.h"
#include "kCalendar.h"

	
void kCalendar::initialize() {
	m_lastCheckedJobId = -1;
	m_workTime = 0;
}

void kCalendar::startFirstJob() {
	m_lastEndTime = millis();
}

void kCalendar::update() {
	m_workTime += Timer::getInstance()->delta();
	for (long long i = m_lastCheckedJobId + 1; i < m_count; i++) {
		kJob job = m_jobs[i];
		if (m_workTime - m_lastEndTime >= job.atTime) {
			job.func();
			if (i == m_count - 1) {
				m_lastCheckedJobId = -1;
				startFirstJob();
			} else 
				m_lastCheckedJobId++;
		} else break;
	}
}

void kCalendar::addJob(void (*func)(), unsigned long time) {
	kJob job = {func, time};
	m_count++;
	kJob *jobs = new kJob[m_count];
	if (m_count > 1)
		for (long long i = 0; i < m_count - 1; i++)
			jobs[i] = m_jobs[i];
	
	jobs[m_count - 1] = job;
	delete m_jobs;
	m_jobs = jobs;
}