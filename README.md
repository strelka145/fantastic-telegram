# fantastic-telegram
A CLI program that retrieves abstracts, authors, references, and other information from the URLs and DOIs of articles.

 ## Install

You must have libcurl or libcurl-dev installed beforehand.

 ```
git clone https://github.com/strelka145/fantastic-telegram.git
cd fantastic-telegram
g++ ./main.cpp ./getinfo.cpp -lcurl -o fantastic-telegram
```

On mac, you may need to add the `--sysroot=$(xcrun --show-sdk-path)` option when building with g++.

## Usage

```
(path to fantastic-telegram) [options]
```

### option

```
Either -u(--url=) or -i(--doi=) must have a value.

	-u(--url)
	-u [argument] or --url=[argument] to obtain the paper information of the specified URL. (If no argument is specified, the URL obtained from DOI is displayed.)

	-i(--doi)
	-i [argument] or --doi=[argument] to obtain the paper information of the specified DOI. (If no argument is specified, the DOI obtained from URL is displayed.)

	-a(--abstract)
	Display abstract.

	-f(--first)
	Display first author.

	-w(--author)
	Display all authors.

	-t(--title)
	Display title.

	-y(--year)
	Display the year of publication.

	-m(--month)
	Display the month of publication.

	-d(--day)
	Display the day of publication.

	-j(--journal)
	Display the journal name.

	-p(--publisher)
	Display publishers.

	-c(--citation)
	Create a citation in the specified format.

	-r(--reference)
	List the DOIs of the references. References for which DOIs cannot be obtained will be skipped.
  ```

### Example

```
$./fantastic-telegram --url="https://www.cell.com/fulltext/S0092-8674(06)00976-7" --author
Kazutoshi Takahashi
Shinya Yamanaka
```

```
$./fantastic-telegram --doi="10.1038/171737a0" -a
The determination in 1953 of the structure of deoxyribonucleic acid (DNA), with its two entwined helices and paired organic bases, was a tour de force in X-ray crystallography. But more significantly, it also opened the way for a deeper understanding of perhaps the most important biological process. In the words of Watson and Crick: &quot;It has not escaped our notice that the specific pairing that we have postulated immediately suggests a possible copying mechanism for the genetic material.&quot; [Obituary of Francis Crick: Nature 430, 845-847 (2004); obituary of Maurice Wilkins: Nature 431, 922 (2004)]
```

### About this application

I created this program using c++ for a little work. The license for this code is not Beer-Ware, but if this was of any help, please buy me candy.
