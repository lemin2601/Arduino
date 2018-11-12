//link tham khảo : http://arduino.vn/bai-viet/565-tao-mot-quy-trinh-cong-nghiep-voi-cac-buoc-bang-arduino
#include "Timer.h"
#include "kCalendar.h"


int i=0,j=0;
//job1
void job1() {
	unsigned long time = millis();
	Serial.print("Job1: ");
  Serial.println(time);
 for(i =0;i<1000;i++){
  for(j =0;j<900;j++){
  if(i==999&&j==800){
    Serial.println("Da for xong");
  }
 }
 }

}

//job2
void job2() {
	unsigned long time = millis();
	Serial.print("Job2: ");
	Serial.println(time);
}

void delayJob() {
	// nothing
}

void setup()
{
	
	//Khởi tạo serial ở mức baudrate 115200
	Serial.begin(115200);

	//Khởi gạo class timer (design pattern singleton) - bắt buộc phải có trong hàm setup (trước khi khởi tạo các job)
	Timer::getInstance()->initialize();

	//Khởi tạo lịch, các công việc phải được sắp xếp theo chiều thời gian tăng dần để tránh lỗi
	kCalendar::getInstance()->initialize();
	//Chạy lệnh job1 ở thời điểm 0 trong chu kỳ
	kCalendar::getInstance()->addJob(job1, 0);
	//Chạy lệnh job2 ở thời điểm 1000ms trong chu kỳ
	kCalendar::getInstance()->addJob(job2, 10);
	
	//Nếu muốn có một khoảng delay trước khi bắt đầu một vòng đời mới ta thêm lệnh này vào chu kỳ. Hiểu một cách nôm na trong trường hợp này là delay 2000ms sau khi thực hiện job
	kCalendar::getInstance()->addJob(delayJob, 1000);

	//Bắt đầu tính giờ thời gian của quy trình đầu tiên
	kCalendar::getInstance()->startFirstJob();
}

//trong hàm loop chỉ nên có những hàm này, bạn muốn viết một chức năng khác? Xin hãy tạo một job và đưa vào thời khóa biểu scheduler như hàm dưới

void loop()
{
	//đầu hàm loop phải có để cập nhập thời điểm diễn ra việc kiểm tra lại các tiến trình
	Timer::getInstance()->update();
	
	kCalendar::getInstance()->update();

	//cuối hàm loop phải có để cập nhập lại THỜI ĐIỂM (thời điểm chứ ko phải thời gian nha, tuy tiếng Anh chúng đều là time) để cho lần xử lý sau
	Timer::getInstance()->resetTick();
	
}
