#include AmbrosioTortorelliMinimizer.hpp

/*

https://github.com/jacobgil/Ambrosio-Tortorelli-Minimizer/blob/master/AmbrosioTortorelliMinimizer.py
Solve for the edge set Z while fixing J, and then solve for J while fixing Z.
*/

AmbrosioTortorelliMinimizer::AmbrosioTortorelliMinimizer(cv::Mat img, unsigned int iter, unsigned int solvIter, float tol, float alpha, float beta, float epsilon): 
							_iter(iter),
							_maxIter(solvIter),
							_tolerance(tol),
							_alpha(alpha), 
							_beta(beta), 
							_epsilon(epsilon), 
							add_const(beta/(4*epsilon)), 
							multiply_const(beta*epsilon)
{
	_g = img/max(img);
	_f = _g;
	_edges = cv::zeros(img);
	updateGradient();
}

void AmbrosioTortorelliMinimizer::updateGradient( void )
{
	gradients( _f, &_gradX, &_gradY);
	_gradMagnitude = _gradX*_gradX + _gradY*_gradY;
}


cv::Mat AmbrosioTortorelliMinimizer::edgeLinearOperator( cv::Mat input )
{
	cv::Mat v = input.reshape( _g.channels(), _g.size()[1] ); // reshape (channel, rows), columns are figured automatically; size() returns [cols, rows]
	cv::Mat laplacian;
	unsigned int kernelSize = 3, scale = 1, delta = 0;
	cv::Laplacian( v, laplacian, CV_16S, kernelSize ,scale, delta, BORDER_DEFAULT );

	cv::Mat res = _gradMagnitude*_alpha*add_const * v - multiply_const* laplacian;
	return res.reshape( input.channels(), input.size()[1] );
}

cv::Mat AmbrosioTortorelliMinimizer::imageLinearOperator( cv::Mat )
{
	_f = input.reshape(  _g.channels(), _g.size()[1] );
	cv::Mat x, y;
	gradients(_f, &x, &y);

	cv::Mat res = f- 2*_alpha*( gradX( _edges*x ) + gradY( _edges*y ) );
	return res.reshape( input.channels(), input.size()[1] );
}

cv::Mat AmbrosioTortorelliMinimizer::solveEdges( void )
{
	unsigned int size = _g.size()[0] * _g.size()[1];
	A = LinearOperator( (size,size), edgeLinearOperator, CV_64F );
	b = cv:ones(size) * add_const;

	//Conjugate Gradient iteration to solve A x = b
	_edges = conjugateGradient( A, b, _tolerance, _maxIter).reshape(  _g.channels(), _g.size()[1] );
	_edges =  _edges*_edges;
	return _edges;
}

cv::Mat AmbrosioTortorelliMinimizer::solveImage( void )
{
	unsigned int size = _g.size()[0] * _g.size()[1];
	A = LinearOperator( (size,size), imageLinearOperator, CV_64F );
	b = _g.reshape( _g.channels(), size ); // must not be inplace reshaping

	//Conjugate Gradient iteration to solve A x = b
	_f = conjugateGradient( A, b, _tolerance, _maxIter).reshape(  _g.channels(), _g.size()[1] );
	updateGradient();
	return _f;
}

cv::Mat AmbrosioTortorelliMinimizer::gradX(cv::Mat img)
{
	cv::Mat kernel<int>(3,3) << -1,0,1,-1,0,1,-1,0,1;
	return cv::filter2D(img, CV_64F, kernel);
}

cv::Mat AmbrosioTortorelliMinimizer::gradY(cv::Mat)
{
	cv::Mat kernel<int>(3,3) << -1,-1,-1,0,0,0,1,1,1;
	return cv::filter2D(img, CV_64F, kernel);
}

cv::Mat[2] AmbrosioTortorelliMinimizer::gradients(cv::Mat)
{
	return {gradX(img), gradY(img)};
}

void AmbrosioTortorelliMinimizer::minimize( void )
{
	for (int i = 0; i < _iter; ++i)
	{
		solveEdges();
		solveImage();
	}

	_edges = cv::pow(_edges,0.5);
	cv::normalize(_f, _f,0,255, CV::NORM_MINMAX);
	cv::normalize(_g, _g,0,255, CV::NORM_MINMAX);
	_f = uint8(_f);
	_edges = 255 - uint8(_edges);

}

void AmbrosioTortorelliMinimizer::getEdges( void )
{
	return _edges;
}
void AmbrosioTortorelliMinimizer::getF( void )
{
	return _f;
}
