#ifndef OUTPUTFORMAT_H
#define OUTPUTFORMAT_H

namespace Utils {

enum OutputFormat
{
	NormalMessageFormat,
	ErrorMessageFormat,
	DebugFormat,
	StdOutFormat,
	StdErrFormat,
	StdOutFormatSameLine,
	StdErrFormatSameLine,
	NumberOfFormats // Keep this entry last.
};

} // namespace Utils

#endif // OUTPUTFORMATR_H
