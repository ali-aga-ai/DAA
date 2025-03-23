import React, { useState } from 'react';
import { BarChart, Bar, XAxis, YAxis, CartesianGrid, Tooltip, Legend, ResponsiveContainer } from 'recharts';

const Dashboard = () => {
  const [selectedDataset, setSelectedDataset] = useState('all');
  const [maxCliqueSize, setMaxCliqueSize] = useState(20); // Default to show cliques up to size 20
  
  // Hardcoded data
  const datasets = [
    { id: 'Wiki', name: 'Wiki' },
    { id: 'Email-enron', name: 'Email-enron' },
    { id: 'Skitter', name: 'Skitter' },
  ];
  
  const algorithms = [
    { id: 'algo1', name: 'ELS' },
    { id: 'algo2', name: 'Tomita' },
    { id: 'algo3', name: 'CHIBA' },
  ];
  
  const cliqueStats = {
    "Wiki": { largestClique: 17, totalMaximalCliques: 459002 },
    "Email-enron": { largestClique: 20, totalMaximalCliques: 226859 },
    "Skitter": { largestClique: 67, totalMaximalCliques: 37322355 },
  };
  
  const executionTimeData = [
    { name: "Wiki", 'ELS': 7.82, 'Tomita': 3.46, 'CHIBA': 2753 },
    { name: "Email-enron", 'ELS': 9.00, 'Tomita': 7.63, 'CHIBA': 4082 },
    { name: "Skitter", 'ELS': 1243.2, 'Tomita': 57618.0, 'CHIBA': 91182 },
  ];
  
  // Updated with complete data
  const cliqueSizeDistributionData = {
    Wiki: [
      { size: 2, count: 8655 },
      { size: 3, count: 13718 },
      { size: 4, count: 27292 },
      { size: 5, count: 48416 },
      { size: 6, count: 68872 },
      { size: 7, count: 83266 },
      { size: 8, count: 76732 },
      { size: 9, count: 54456 },
      { size: 10, count: 35470 },
      { size: 11, count: 21736 },
      { size: 12, count: 11640 },
      { size: 13, count: 5449 },
      { size: 14, count: 2329 },
      { size: 15, count: 740 },
      { size: 16, count: 208 },
      { size: 17, count: 23 },
    ],
    'Email-enron': [
      { size: 2, count: 14070 },
      { size: 3, count: 7077 },
      { size: 4, count: 13319 },
      { size: 5, count: 18143 },
      { size: 6, count: 22715 },
      { size: 7, count: 25896 },
      { size: 8, count: 24766 },
      { size: 9, count: 22884 },
      { size: 10, count: 21393 },
      { size: 11, count: 17833 },
      { size: 12, count: 15181 },
      { size: 13, count: 11487 },
      { size: 14, count: 7417 },
      { size: 15, count: 3157 },
      { size: 16, count: 1178 },
      { size: 17, count: 286 },
      { size: 18, count: 41 },
      { size: 19, count: 10 },
      { size: 20, count: 6 },
    ],
    Skitter: [
      { size: 2, count: 2319807 },
      { size: 3, count: 3171609 },
      { size: 4, count: 1823321 },
      { size: 5, count: 939336 },
      { size: 6, count: 684873 },
      { size: 7, count: 598284 },
      { size: 8, count: 588889 },
      { size: 9, count: 608937 },
      { size: 10, count: 665661 },
      { size: 11, count: 728098 },
      { size: 12, count: 798073 },
      { size: 13, count: 877282 },
      { size: 14, count: 945194 },
      { size: 15, count: 980831 },
      { size: 16, count: 939987 },
      { size: 17, count: 839330 },
      { size: 18, count: 729601 },
      { size: 19, count: 639413 },
      { size: 20, count: 600192 },
      { size: 21, count: 611976 },
      { size: 22, count: 640890 },
      { size: 23, count: 673924 },
      { size: 24, count: 706753 },
      { size: 25, count: 753633 },
      { size: 26, count: 818353 },
      { size: 27, count: 892719 },
      { size: 28, count: 955212 },
      { size: 29, count: 999860 },
      { size: 30, count: 1034106 },
      { size: 31, count: 1055653 },
      { size: 32, count: 1017560 },
      { size: 33, count: 946717 },
      { size: 34, count: 878552 },
      { size: 35, count: 809485 },
      { size: 36, count: 744634 },
      { size: 37, count: 663650 },
      { size: 38, count: 583922 },
      { size: 39, count: 520239 },
      { size: 40, count: 474301 },
      { size: 41, count: 420796 },
      { size: 42, count: 367879 },
      { size: 43, count: 321829 },
      { size: 44, count: 275995 },
      { size: 45, count: 222461 },
      { size: 46, count: 158352 },
      { size: 47, count: 99522 },
      { size: 48, count: 62437 },
      { size: 49, count: 39822 },
      { size: 50, count: 30011 },
      { size: 51, count: 25637 },
      { size: 52, count: 17707 },
      { size: 53, count: 9514 },
      { size: 54, count: 3737 },
      { size: 55, count: 2042 },
      { size: 56, count: 1080 },
      { size: 57, count: 546 },
      { size: 58, count: 449 },
      { size: 59, count: 447 },
      { size: 60, count: 405 },
      { size: 61, count: 283 },
      { size: 62, count: 242 },
      { size: 63, count: 146 },
      { size: 64, count: 84 },
      { size: 65, count: 49 },
      { size: 66, count: 22 },
      { size: 67, count: 4 },
    ],
  };
  
  const colors = {
    Wiki: '#8884d8',
    'Email-enron': '#82ca9d',
    Skitter: '#ffc658',
  };
  
  // Get filtered data based on selected dataset and max clique size
  const getFilteredData = () => {
    if (selectedDataset === 'all') {
      return {
        Wiki: cliqueSizeDistributionData.Wiki.filter(entry => entry.size <= maxCliqueSize),
        'Email-enron': cliqueSizeDistributionData['Email-enron'].filter(entry => entry.size <= maxCliqueSize),
        Skitter: cliqueSizeDistributionData.Skitter.filter(entry => entry.size <= maxCliqueSize)
      };
    } else {
      return {
        [selectedDataset]: cliqueSizeDistributionData[selectedDataset].filter(entry => entry.size <= maxCliqueSize)
      };
    }
  };
  
  const filteredDistributionData = getFilteredData();
  
  // Generate summary table data
  const generateSummaryData = () => {
    return datasets.map(dataset => {
      const datasetCliques = cliqueSizeDistributionData[dataset.id];
      const totalCliques = cliqueStats[dataset.id].totalMaximalCliques;
      const largestSize = cliqueStats[dataset.id].largestClique;
      
      // Find the most common clique size
      const mostCommonSize = datasetCliques.reduce((max, current) => 
        current.count > max.count ? current : max, datasetCliques[0]);
        
      return {
        name: dataset.name,
        totalCliques: totalCliques.toLocaleString(),
        largestSize,
        mostCommonSize: `${mostCommonSize.size} (${mostCommonSize.count.toLocaleString()})`,
        smallCliques: datasetCliques.filter(c => c.size <= 5)
          .reduce((sum, current) => sum + current.count, 0).toLocaleString(),
        mediumCliques: datasetCliques.filter(c => c.size > 5 && c.size <= 15)
          .reduce((sum, current) => sum + current.count, 0).toLocaleString(),
        largeCliques: datasetCliques.filter(c => c.size > 15)
          .reduce((sum, current) => sum + current.count, 0).toLocaleString(),
      };
    });
  };
  
  const summaryData = generateSummaryData();
  
  return (
    <div className="bg-gray-50 min-h-screen p-6">
      <div className="max-w-7xl mx-auto">
        <h1 className="text-3xl font-bold text-gray-800 mb-6">Clique Analysis Dashboard</h1>
        
        {/* Overview Cards */}
        <div className="grid grid-cols-1 md:grid-cols-3 gap-6 mb-8">
          {datasets.map(dataset => (
            <div key={dataset.id} className="bg-white rounded-lg shadow p-6">
              <h2 className="text-xl font-semibold mb-4">{dataset.name} Overview</h2>
              <div className="flex justify-between">
                <div className="text-center">
                  <p className="text-sm text-gray-500">Largest Clique Size</p>
                  <p className="text-2xl font-bold text-indigo-600">{cliqueStats[dataset.id].largestClique}</p>
                </div>
                <div className="text-center">
                  <p className="text-sm text-gray-500">Total Maximal Cliques</p>
                  <p className="text-2xl font-bold text-indigo-600">
                    {cliqueStats[dataset.id].totalMaximalCliques.toLocaleString()}
                  </p>
                </div>
              </div>
            </div>
          ))}
        </div>
        
        {/* Execution Time Chart */}
        <div className="bg-white rounded-lg shadow p-6 mb-8">
          <h2 className="text-xl font-semibold mb-4">Execution Time Comparison (seconds)</h2>
          <div className="h-80">
            <ResponsiveContainer width="100%" height="100%">
              <BarChart
                data={executionTimeData}
                margin={{ top: 20, right: 30, left: 20, bottom: 5 }}
                layout="vertical"
              >
                <CartesianGrid strokeDasharray="3 3" />
                <XAxis type="number" />
                <YAxis dataKey="name" type="category" />
                <Tooltip formatter={(value) => `${value.toFixed(2)} seconds`} />
                <Legend />
                <Bar dataKey="ELS" fill="#8884d8" />
                <Bar dataKey="Tomita" fill="#82ca9d" />
                <Bar dataKey="CHIBA" fill="#ffc658" />
              </BarChart>
            </ResponsiveContainer>
          </div>
          <div className="mt-4 text-sm text-gray-500">
           
          </div>
        </div>
        
        {/* Clique Size Distribution */}
        <div className="bg-white rounded-lg shadow p-6 mb-8">
          <div className="flex flex-col md:flex-row justify-between items-start md:items-center mb-4 gap-4">
            <h2 className="text-xl font-semibold">Clique Size Distribution</h2>
            <div className="flex flex-col md:flex-row gap-4">
              <div className="flex flex-wrap gap-2">
                <button
                  onClick={() => setSelectedDataset('all')}
                  className={`px-3 py-1 rounded text-sm ${selectedDataset === 'all' ? 'bg-indigo-600 text-white' : 'bg-gray-200 text-gray-700'}`}
                >
                  All Datasets
                </button>
                {datasets.map(dataset => (
                  <button
                    key={dataset.id}
                    onClick={() => setSelectedDataset(dataset.id)}
                    className={`px-3 py-1 rounded text-sm ${selectedDataset === dataset.id ? 'bg-indigo-600 text-white' : 'bg-gray-200 text-gray-700'}`}
                  >
                    {dataset.name}
                  </button>
                ))}
              </div>
              <div className="flex items-center gap-2">
                <span className="text-sm text-gray-700">Max Size:</span>
                <select 
                  value={maxCliqueSize}
                  onChange={(e) => setMaxCliqueSize(parseInt(e.target.value))}
                  className="px-2 py-1 border rounded text-sm bg-white"
                >
                  <option value="10">10</option>
                  <option value="20">20</option>
                  <option value="30">30</option>
                  <option value="40">40</option>
                  <option value="67">All (up to 67)</option>
                </select>
              </div>
            </div>
          </div>
          
          <div className="grid grid-cols-1 md:grid-cols-3 gap-6">
            {selectedDataset === 'all' ? (
              Object.entries(filteredDistributionData).map(([datasetId, data]) => (
                <div key={datasetId} className="h-64">
                  <h3 className="text-center text-gray-700 font-medium mb-2">
                    {datasetId}
                  </h3>
                  <ResponsiveContainer width="100%" height="100%">
                    <BarChart data={data}>
                      <CartesianGrid strokeDasharray="3 3" />
                      <XAxis dataKey="size" label={{ value: 'Clique Size', position: 'insideBottom', offset: -5 }} />
                      <YAxis label={{ value: 'Count', angle: -90, position: 'insideLeft' }} />
                      <Tooltip formatter={(value) => value.toLocaleString()} />
                      <Bar dataKey="count" fill={colors[datasetId]} />
                    </BarChart>
                  </ResponsiveContainer>
                </div>
              ))
            ) : (
              <div className="col-span-3 h-80">
                <ResponsiveContainer width="100%" height="100%">
                  <BarChart data={filteredDistributionData[selectedDataset]}>
                    <CartesianGrid strokeDasharray="3 3" />
                    <XAxis dataKey="size" label={{ value: 'Clique Size', position: 'insideBottom', offset: -5 }} />
                    <YAxis label={{ value: 'Count', angle: -90, position: 'insideLeft' }} />
                    <Tooltip formatter={(value) => value.toLocaleString()} />
                    <Bar dataKey="count" fill={colors[selectedDataset]} />
                  </BarChart>
                </ResponsiveContainer>
              </div>
            )}
          </div>
        </div>
        
        {/* Clique Size Distribution Summary */}
        <div className="bg-white rounded-lg shadow p-6 mb-8">
          <h2 className="text-xl font-semibold mb-4">Clique Distribution Summary</h2>
          <div className="overflow-x-auto">
            <table className="min-w-full divide-y divide-gray-200">
              <thead className="bg-gray-50">
                <tr>
                  <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Dataset</th>
                  <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Total Cliques</th>
                  <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Largest Size</th>
                  <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Most Common Size</th>
                  <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Small Cliques (≤5)</th>
                  <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Medium Cliques (6-15)</th>
                  <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Large Cliques (15)</th>
                </tr>
              </thead>
              <tbody className="bg-white divide-y divide-gray-200">
                {summaryData.map((dataset) => (
                  <tr key={dataset.name}>
                    <td className="px-6 py-4 whitespace-nowrap text-sm font-medium text-gray-900">{dataset.name}</td>
                    <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500">{dataset.totalCliques}</td>
                    <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500">{dataset.largestSize}</td>
                    <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500">{dataset.mostCommonSize}</td>
                    <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500">{dataset.smallCliques}</td>
                    <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500">{dataset.mediumCliques}</td>
                    <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500">{dataset.largeCliques}</td>
                  </tr>
                ))}
              </tbody>
            </table>
          </div>
        </div>
        
        {/* Algorithm Performance Summary */}
        <div className="bg-white rounded-lg shadow p-6">
          <h2 className="text-xl font-semibold mb-4">Algorithm Performance Summary</h2>
          <div className="overflow-x-auto">
            <table className="min-w-full divide-y divide-gray-200">
              <thead className="bg-gray-50">
                <tr>
                  <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Dataset</th>
                  <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Fastest Algorithm</th>
                  <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Time (s)</th>
                  <th className="px-6 py-3 text-left text-xs font-medium text-gray-500 uppercase tracking-wider">Improvement Over Slowest</th>
                </tr>
              </thead>
              <tbody className="bg-white divide-y divide-gray-200">
                {executionTimeData.map((dataset) => {
                  const times = {
                    'ELS': dataset['ELS'],
                    'Tomita': dataset['Tomita'],
                    'CHIBA': dataset['CHIBA']
                  };
                  console.log(times);
                  const fastestAlgo = Object.entries(times).reduce((a, b) => a[1] < b[1] ? a : b);
                  const slowestTime = Math.max(...Object.values(times));
                  const improvement = ((slowestTime - fastestAlgo[1]) / slowestTime * 100).toFixed(1);
                  
                  return (
                    <tr key={dataset.name}>
                      <td className="px-6 py-4 whitespace-nowrap text-sm font-medium text-gray-900">{dataset.name}</td>
                      <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500">{fastestAlgo[0]}</td>
                      <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500">{fastestAlgo[1].toFixed(2)}</td>
                      <td className="px-6 py-4 whitespace-nowrap text-sm text-gray-500">{improvement}%</td>
                    </tr>
                  );
                })}
              </tbody>
            </table>
          </div>
        </div>
      </div>
    </div>
  );
};

export default Dashboard;
