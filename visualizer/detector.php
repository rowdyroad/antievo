<?php
error_reporting(E_ALL);
ini_set('display_errors',1);


class WindowDeviation
{
	private $window_size_;
	private $window_index_ = 0;
	private $index_ = 0;
	private $window_ = [];

	public function __construct($window_size)
	{
		$this->window_size_ = $window_size;
	}

	public function process($value)
	{
		$ret = false;
		if ($this->window_index_ < $this->window_size_) {
			$this->window_[$this->window_index_++] = floatval($value);
		} else {
			$this->window_ = array_slice($this->window_, 1);
			$this->window_[$this->window_index_ - 1] = floatval($value);
		    $mean = array_sum($this->window_) / $this->window_size_;
		    $carry = 0.0;
		    foreach ($this->window_ as $val) {
		        $d = ((double) $val) - $mean;
		      	$carry += $d * $d;
		    }
		    $ret = [$this->index_ - $this->window_size_, round(sqrt($carry / $this->window_size_), 5)];
		}
		$this->index_++;
		return $ret;
	}
};


$filename = $argv[1];
$data = file_get_contents($filename);

if (preg_match_all('/\d+\s+[0-9\.-]+\s+[0-9\.-]+\s+([0-9\.-]+).+/', $data,$regs)) {
	$items = [];
	$wd = new WindowDeviation(100);
	foreach ($regs[1] as $value) {
		$ret = $wd->process($value);
		if ($ret !== false) {
			list($index, $st) = $ret;
			printf("%d - %f\n", $index, $st);
			if ($st > 0) {
				$items[] = $value;
				if (count($items) >= 1000) {
					$mean = round(array_sum($items) / count($items), 2);
					$wd2 = new WindowDeviation(100);
					foreach ($items as $i=>$item) {
						$result = $wd2->process(abs($item - $mean) <= 0.01 ? $mean : $item);
						if ($result !== false) {
							//printf("%d\t%f\n", $i + $index, $result[1]);
						}
					}
					exit(0);
				}
			} else {
				$items = [];
			}
		}
	}
}
