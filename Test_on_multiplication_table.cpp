/* Написать программу проверки знания таблицы умножения. 
Программа должна вывести 10 примеров и выставить оценку: 
за 10 правильных ответов — «отлично», 
за 8 или 9 правильных ответов — «хорошо», 
за 6 или 7 правильных ответов — «удовлетворительно», 
остальные варианты — «плохо».*/

#include <iostream>
#include <ctime>
#include <chrono>         // для std::chrono
#include <Windows.h>      // для sleep()
#include <conio.h>        //для _kbhit (ожидание нажатия клавиши)

using std::cout; using std::cin; using std::endl;

// класс с функцией счётчика времени из 129 урока
class Timer
{
private:
	using clock_t = std::chrono::high_resolution_clock;
	using second_t = std::chrono::duration<double, std::ratio<1>>;
	std::chrono::time_point<clock_t> m_beg;  // начало временного интервала
public:
	Timer() : m_beg(clock_t::now())   // при создании объекта класса начало временного интервала определяем как "сейчас"
	{
	}
	void reset() // метод "перезагрузки", когда опять засекаем время
	{
		m_beg = clock_t::now();
	}
	double elapsed() const // метод считает пройденное время от начала временного интервала
	{
		return std::chrono::duration_cast<second_t>(clock_t::now() - m_beg).count();
	}
};

// получаем ответ от пользователя 
unsigned get_answer()
{
	int answer;
	cin >> answer;
	while (cin.fail() || (answer < 0))
	{
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Ответ не корректен, повторите ввод: ";
		cin >> answer;
	}
	return static_cast<unsigned>(answer);
}

// функция генерирует пример о обрабатывает ответ на него. Если ответ правильный - возвращает true, если неправильный или время истекло - ложь
bool work_with_example(const unsigned time_for_answer)
{
	unsigned number1{ static_cast<unsigned>(rand()) % 9 + 2 };        // число от 2 до 10
	unsigned number2{ static_cast<unsigned>(rand()) % 9 + 2 };        // число от 2 до 10
	cout << number1 << " * " << number2 << " = ";
	bool time_finished{ false };
	Timer t;                                  // засекаем время, создавая объект класса Timer
	do
	{
		time_finished = (t.elapsed() >= time_for_answer);
		if (time_finished) break;
	} while (!_kbhit());                      // пока пользователь не начнёт вводить ответ
	if (time_finished)                        // если время вышло
	{
		cout << "\a" << "Время на ответ истекло!" << endl;
		return false;
	}
	else                                      // если пользователь ввёл ответ
	{
		if ((number1 * number2) == get_answer())
		{
			cout << "Верно!\n";
			return true;
		}
		else
			cout << "Вы ошиблись! Правильный ответ: " << number1 * number2 << '\n';
		return false;
	}
}

// функция даёт пользователю обратную связь по окончанию теста
void evaluate_test(const unsigned short correct_answers)
{
	cout << "Тест окончен! Ваша оценка: ";
	switch (correct_answers)
	{
	case 10:
		cout << "5 (отлично)\n";
		break;
	case 9:
	case 8:
		cout << "4 (хорошо)\n";
		break;
	case 7:
	case 6:
		cout << "3 (удовлетворительно)\n";
		break;
	default:
		cout << "2 (плохо)\n";
	}
}

int main()
{
	setlocale(LC_CTYPE, "rus");                    // для вывода сообщений на кириллице
	srand(static_cast<unsigned int>(time(0)));     // аналог randomize с привязкой ко времени запуска

	const unsigned short n_examples{ 10 };         // количество вопросов
	const unsigned time_for_answer{ 3 };           // время в секундах на каждый ответ
	cout << "Программа проверяет знание таблицы умножения,\n" 
		 << "всего будет " << n_examples << " вопросов, на каждый ответ у Вас есть " << time_for_answer << " секунды!\n";

	Sleep(3000);                                   // даём пользователю прочитать сообщение

	unsigned short correct_answers{ 0 };
	for (unsigned short i = 0; i < n_examples; ++i)
	{
		cout << i + 1 << "): ";
		if (work_with_example(time_for_answer) == true) 
			++correct_answers;
	}
	evaluate_test(correct_answers);
	return 0;
}