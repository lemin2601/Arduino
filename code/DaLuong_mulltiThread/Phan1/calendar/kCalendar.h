#ifndef __KCALENDAR__
#define __KCALENDAR__

//Đây là một class dùng kiểu thiết kế hướng đối tượng singleton
struct kJob {
	void (*func)();
	unsigned long atTime;
};
class kCalendar {
private:
	long long m_count;
	long long m_lastCheckedJobId;
	unsigned long m_workTime;
	unsigned long m_lastEndTime;
	kJob *m_jobs;
public:
	static kCalendar* getInstance() {
		static kCalendar *instance = new kCalendar();
		return instance;
	}
	
	void initialize();
	
	void startFirstJob();
	
	void update();
	
	void addJob(void (*func)(), unsigned long time);
};
#endif