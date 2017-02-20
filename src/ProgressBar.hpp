#ifndef __pbar__
#define __pbar__

#include <iostream>

class ProgressBar
{
	private:
	int* _current;
	int* _max;
	int _size;
	std::string _format;

	public:
	ProgressBar( int* i, int* m, std::string f="[#>-]", int s=50)
	{
		_current = i;
		_max = m;
		_size = s;
		_format = f;
		
		if(f.length()!=4 && f.length()!=5) 
		{
			std::cerr<<"ProgressBar::format\tInvalid format: length must be 4 or 5. Using default '[#>-]'"<<std::endl;
			_format="[#-]";
		}
	}
	
	~ProgressBar()
	{
		_current = nullptr;
		_max = nullptr;
	}
	
	void SetSize(int s)
	{
		_size = s;
	}
	
	void SetFormat(std::string f)
	{
		if(f.length()!=4 && f.length()!=5) 
		{
			std::cerr<<"ProgressBar::format\tInvalid format: length must be 4 or 5. Using default '[#>-]'"<<std::endl;
			f="[#>-]";
		}
		
		_format = f;
		
	}
	
	void Progress()
	{		
		int iter = *_current+1;
		int maxIter = *_max;
	
		if(iter<=maxIter)
		{
		
			int l = _format.length()-1;
			
			int percentage = 100*iter/maxIter;
			int scaledPercentage = _size*iter/maxIter;
			
			std::cout<<"\r"<<_format[0];
			for(int i=0;i<=scaledPercentage-1;i++) std::cout<<_format[1];
			std::cout<<_format[l-2];
			for(int i=scaledPercentage+1;i<_size;i++) std::cout<<_format[l-1];
			std::cout<<_format[l]<<"\t"<< iter <<"/"<<maxIter<<"\t("<<percentage<<"%)"<<std::flush;
			
			if(percentage==100)
			std::cout<<std::endl;
		}
		
	}


};

#endif
