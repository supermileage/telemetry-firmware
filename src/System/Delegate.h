#ifndef DELEGATE
#define DELEGATE

class Delegate {
	public:
		Delegate() { }

		virtual ~Delegate() = 0;

		virtual void* execute(void* arg) = 0;
};

#endif