#ifndef __AMBROSIOTORTORELLIMINIMIZER_HPP__
#define __AMBROSIOTORTORELLIMINIMIZER_HPP__


#endif

class AmbrosioTortorelliMinimizer
{
private:
	int _iterations;
	float tol = _tol;//?
	cv::Mat _g, _f;//normalized image
	cv::Mat _edges = np.zeros(img.shape);
	cv::Mat _gradX, _gradY, _gradMagnitude;
	float _alpha, _beta, _epsilon;
	const float add_const, multiply_const;
	unsigned int _maxiter;

	void updateGradient( void );
	cv::Mat edgeLinearOperator( cv::Mat );
	cv::Mat imageLinearOperator( cv::Mat );
	cv::Mat solveImage( void );
	cv::Mat solveEdges( void );

	std::Function gradX(cv::Mat);
	std::Function gradY(cv::Mat);
	std::Function gradients(cv::Mat);
public:

	AmbrosioTortorelliMinimizer(cv::Mat, unsigned int, unsigned int, float, float, float, float);
	void minimize( cv::Mat*, cv::Mat* );

}