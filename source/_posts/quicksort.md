---
title: quicksort
date: 2018-04-30
tags:
    - golang
---

### 快速排序时为什么一定要把pivot放到头或尾最后在放回去呢？
因为我们在把头尾index往里缩时我们判断的是小于和不小于(大于同理)，比如我们把pivot 5 放在中间，当我们的头index判断不小于时，若是到了pivot条件也是成立的就跳到了pivot右侧，此时若是尾index也在右边发现了小于pivot的值在交换时就会出现，小于pivot的值出现在pivot的右侧，当然我们也可以做额外操作，判断若是头index到了pivot的位置时则不再继续
```
1. 
   |     | 
   V     V
4, 5, 8, 2
  头      尾

2. 
      |  | 
      V  V
4, 5, 2, 8
      头 尾

3. 此时找到了大于以及小于的pivot的值交换
4，5，2，8， 显然出问题了
、、、、、、
但是若是5在头部
5，4，8，2， 交换就没事了，因为接下来会有把pivot和low或者high交换的过程
```
### 我们每进行一次交换就会找到一个元素的正确位置，此时接下来再排序时就不要再把他算进去了
```
2,1,2,1,3,4,5
```
上述3已经在正确位置了，再接下来的排序应当是4,5和2，1，2，1两组进行排序而不是3，4，5和2，1，2，1或者4，5和2，1，2，1, 3进行排序, 因为3已经是在正确位置了，没必要再排一次。

### 我们可看看golang源码的排序
1. 若是小于12个元素时它采用shell希尔排序
2. 他会计算一个深度2 * lg(n+1), 然后采用快速排序，没进行一次就深度减一，当为0时就用堆排序,至于这个数字咋算出来的可以查查

解析：元素较少时，大家都差不多，反而是希尔排序这样的好一些，好在哪啊？
我们都知道快排在元素基本有序时是最慢的最坏能达到o(n^2),所以当序列基本有序时我们可以采用堆排序

### 一个简单的快排版本
```
package main
import (
	"fmt"
)
 
// threads 线程标识创建线程的个数
func quicksort(nums []int, length int) {
	if length <= 1 {
		return
	}
	
	left := 1
	right := length - 1
	
	pivot := nums[0]
	
  for{
		for; left < right && nums[right] > pivot; right--{ }
		for; left < right && nums[left] <= pivot; left++ { }
		if left >= right {
			break
		}
		nums[left], nums[right] = nums[right], nums[left]
	}
	nums[0], nums[left] = nums[left], nums[0]

	quicksort(nums[0:left], left) //分任务
	temp := nums[left + 1: length]
  quicksort(temp, len(temp))
  return
}

func main() {
	x := []int{3, 41, 24, 76, 11, 45, 3, 3, 64, 21, 69, 19, 36}
	quicksort(x[:], len(x))
	fmt.Println(x);
}
```