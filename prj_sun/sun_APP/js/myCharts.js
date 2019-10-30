var option = {
	title: {
		text: '温度历史数据'
	},
	tooltip: {},
	legend: {
		data:['温度']
	},
	xAxis: {
		data: []
	},
	 dataZoom: [{
		type: 'slider',
		xAxisIndex: 0,
		start: 10,
		end: 60
	},
	{
		type: 'inside',
		xAxisIndex: 0,
		start: 10,
		end: 60
	},
	{
		type: 'slider',
		yAxisIndex: 0,
		start: 30,
		end: 80
	},
	{
		type: 'inside',
		yAxisIndex: 0,
		start: 30,
		end: 80
	}],
	yAxis: {
		type:'value',
		max:50,
		min:0,
	},
	series: [{
		name: '温度',
		type: 'line',
		data: []
	}],
	tooltip: {
	  trigger: 'axis',
	  axisPointer: {    
		type: 'shadow'   
	  },
	},
};
var myChart = echarts.init(document.getElementById('main'));
myChart.setOption(option);