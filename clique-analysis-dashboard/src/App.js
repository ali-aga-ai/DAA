import React, { useState } from 'react';
import { BarChart, Bar, XAxis, YAxis, CartesianGrid, Tooltip, Legend, ResponsiveContainer, LineChart, Line, RadarChart, PolarGrid, PolarAngleAxis, PolarRadiusAxis, Radar } from 'recharts';

const NetworkDensityVisualizer = () => {
  // Dataset from the table
  const data = [
    {
      dataset: 'S-DBLP',
      edge: { total: 6, opt: 22, eds: 22 },
      triangle: { opt: 55, eds: 55 },
      fourClique: { opt: 99, eds: 99 },
      fiveClique: { opt: 132, eds: 132 },
      sixClique: { opt: 73.5, eds: 66 },
      twoStar: { opt: 165, eds: 165 }
    },
    {
      dataset: 'Yeast',
      edge: { total: 3.13, opt: 2.11, eds: 0.467 },
      triangle: { opt: 0.67, eds: 0.0 },
      fourClique: { opt: 0.0, eds: 0.0 },
      fiveClique: { opt: 0.0, eds: 0.0 },
      sixClique: { opt: 111.3, eds: 18.13 },
      twoStar: { opt: 20, eds: 19.2 }
    },
    {
      dataset: 'Netscience',
      edge: { total: 9.50, opt: 57.25, eds: 57.25 },
      triangle: { opt: 242.3, eds: 242.3 },
      fourClique: { opt: 775.2, eds: 775.2 },
      fiveClique: { opt: 1938, eds: 1938 },
      sixClique: { opt: 171, eds: 171 },
      twoStar: { opt: 726.8, eds: 726.8 }
    },
    {
      dataset: 'As-733',
      edge: { total: 8.19, opt: 31.43, eds: 31.35 },
      triangle: { opt: 68.67, eds: 67.94 },
      fourClique: { opt: 92.78, eds: 90.23 },
      fiveClique: { opt: 79.37, eds: 75.13 },
      sixClique: { opt: 826.3, eds: 153.8 },
      twoStar: { opt: 3376, eds: 437.7 }
    }
  ];

  // Prepare data for the pattern comparison
  const prepareComparisonData = (pattern) => {
    return data.map(item => ({
      name: item.dataset,
      opt: item[pattern].opt,
      eds: item[pattern].eds,
      total: item[pattern].total
    }));
  };

  // Prepare data for the radar chart
  const prepareRadarData = () => {
    return data.map(item => ({
      dataset: item.dataset,
      edge: item.edge.opt || 0,
      triangle: item.triangle.opt || 0,
      fourClique: item.fourClique.opt || 0,
      fiveClique: item.fiveClique.opt || 0,
      sixClique: item.sixClique.opt || 0,
      twoStar: item.twoStar.opt || 0
    }));
  };

  // Patterns overview data
  const patternOverviewData = [
    { name: 'Edge', S_DBLP: 22, Yeast: 2.11, Netscience: 57.25, As_733: 31.43 },
    { name: 'Triangle', S_DBLP: 55, Yeast: 0.67, Netscience: 242.3, As_733: 68.67 },
    { name: 'Four-Clique', S_DBLP: 99, Yeast: 0.0, Netscience: 775.2, As_733: 92.78 },
    { name: 'Five-Clique', S_DBLP: 132, Yeast: 0.0, Netscience: 1938, As_733: 79.37 },
    { name: 'Six-Clique', S_DBLP: 73.5, Yeast: 111.3, Netscience: 171, As_733: 826.3 },
    { name: 'Two-Star', S_DBLP: 165, Yeast: 20, Netscience: 726.8, As_733: 3376 }
  ];

  // Diamond pattern data
  const diamondData = [
    { name: 'S-DBLP', opt: 165, eds: 165 },
    { name: 'Yeast', opt: 20, eds: 19.2 },
    { name: 'Netscience', opt: 726.8, eds: 726.8 },
    { name: 'As-733', opt: 3376, eds: 437.7 }
  ];

  // State to track the selected pattern for comparison
  const [selectedPattern, setSelectedPattern] = useState('edge');

  // Chart color scheme
  const colors = {
    opt: '#8884d8',
    eds: '#82ca9d',
    total: '#ffc658',
    S_DBLP: '#8884d8',
    Yeast: '#82ca9d',
    Netscience: '#ffc658',
    As_733: '#ff8042'
  };

  // Domain calculation for charts
  const calculateDomain = (data, key) => {
    if (!data) return [0, 100];
    const values = data.map(item => Math.max(item[key] || 0, item.eds || 0));
    const maxValue = Math.max(...values);
    return [0, maxValue * 1.1]; // Add 10% margin
  };

  return (
    <div className="flex flex-col p-4 gap-8">
      <h1 className="text-2xl font-bold text-center">
        Network Pattern Density Analysis
      </h1>
      
      <div className="bg-gray-50 p-4 rounded-lg shadow">
        <h2 className="text-xl font-semibold mb-4">Pattern-Density Overview</h2>
        <div className="h-96">
          <ResponsiveContainer width="100%" height="100%">
            <BarChart
              data={patternOverviewData}
              margin={{ top: 20, right: 30, left: 20, bottom: 5 }}
            >
              <CartesianGrid strokeDasharray="3 3" />
              <XAxis dataKey="name" />
              <YAxis />
              <Tooltip formatter={(value) => value.toFixed(2)} />
              <Legend />
              <Bar dataKey="S_DBLP" name="S-DBLP" fill={colors.S_DBLP} />
              <Bar dataKey="Yeast" fill={colors.Yeast} />
              <Bar dataKey="Netscience" fill={colors.Netscience} />
              <Bar dataKey="As_733" name="As-733" fill={colors.As_733} />
            </BarChart>
          </ResponsiveContainer>
        </div>
      </div>
      
      <div className="grid grid-cols-1 md:grid-cols-2 gap-6">
        <div className="bg-gray-50 p-4 rounded-lg shadow">
          <h2 className="text-xl font-semibold mb-4">Pattern Comparison</h2>
          <div className="mb-4">
            <select 
              className="p-2 border rounded-md w-full"
              value={selectedPattern}
              onChange={(e) => setSelectedPattern(e.target.value)}
            >
              <option value="edge">Edge</option>
              <option value="triangle">Triangle</option>
              <option value="fourClique">4-Clique</option>
              <option value="fiveClique">5-Clique</option>
              <option value="sixClique">6-Clique</option>
              <option value="twoStar">2-Star</option>
            </select>
          </div>
          <div className="h-64">
            <ResponsiveContainer width="100%" height="100%">
              <BarChart
                data={prepareComparisonData(selectedPattern)}
                margin={{ top: 5, right: 30, left: 20, bottom: 5 }}
              >
                <CartesianGrid strokeDasharray="3 3" />
                <XAxis dataKey="name" />
                <YAxis domain={calculateDomain(prepareComparisonData(selectedPattern), 'opt')} />
                <Tooltip formatter={(value) => value.toFixed(2)} />
                <Legend />
                <Bar dataKey="opt" name="ρ-opt" fill={colors.opt} />
                <Bar dataKey="eds" name="ρ(EDS,Ψ)" fill={colors.eds} />
                {selectedPattern === 'edge' && <Bar dataKey="total" name="ρ-total" fill={colors.total} />}
              </BarChart>
            </ResponsiveContainer>
          </div>
        </div>
        
        <div className="bg-gray-50 p-4 rounded-lg shadow">
          <h2 className="text-xl font-semibold mb-4">Diamond Pattern Comparison</h2>
          <div className="h-64">
            <ResponsiveContainer width="100%" height="100%">
              <BarChart
                data={diamondData}
                margin={{ top: 5, right: 30, left: 20, bottom: 5 }}
              >
                <CartesianGrid strokeDasharray="3 3" />
                <XAxis dataKey="name" />
                <YAxis domain={calculateDomain(diamondData, 'opt')} />
                <Tooltip formatter={(value) => value.toFixed(2)} />
                <Legend />
                <Bar dataKey="opt" name="ρ-opt" fill={colors.opt} />
                <Bar dataKey="eds" name="ρ(EDS,Ψ)" fill={colors.eds} />
              </BarChart>
            </ResponsiveContainer>
          </div>
        </div>
        
        <div className="bg-gray-50 p-4 rounded-lg shadow">
          <h2 className="text-xl font-semibold mb-4">Dataset Radar Comparison</h2>
          <div className="h-64">
            <ResponsiveContainer width="100%" height="100%">
              <RadarChart outerRadius={90} data={prepareRadarData()}>
                <PolarGrid />
                <PolarAngleAxis dataKey="dataset" />
                <PolarRadiusAxis angle={30} domain={[0, 500]} />
                <Radar name="Edge" dataKey="edge" stroke={colors.S_DBLP} fill={colors.S_DBLP} fillOpacity={0.6} />
                <Radar name="Triangle" dataKey="triangle" stroke={colors.Yeast} fill={colors.Yeast} fillOpacity={0.6} />
                <Radar name="4-Clique" dataKey="fourClique" stroke={colors.Netscience} fill={colors.Netscience} fillOpacity={0.6} />
                <Radar name="Two-Star" dataKey="twoStar" stroke={colors.As_733} fill={colors.As_733} fillOpacity={0.6} />
                <Tooltip />
                <Legend />
              </RadarChart>
            </ResponsiveContainer>
          </div>
        </div>
        
        <div className="bg-gray-50 p-4 rounded-lg shadow">
          <h2 className="text-xl font-semibold mb-4">Edge vs Triangle Relationship</h2>
          <div className="h-64">
            <ResponsiveContainer width="100%" height="100%">
              <LineChart
                data={data}
                margin={{ top: 5, right: 30, left: 20, bottom: 5 }}
              >
                <CartesianGrid strokeDasharray="3 3" />
                <XAxis dataKey="dataset" />
                <YAxis yAxisId="left" />
                <YAxis yAxisId="right" orientation="right" />
                <Tooltip />
                <Legend />
                <Line yAxisId="left" type="monotone" dataKey="edge.opt" name="Edge ρ-opt" stroke={colors.opt} activeDot={{ r: 8 }} />
                <Line yAxisId="right" type="monotone" dataKey="triangle.opt" name="Triangle ρ-opt" stroke={colors.eds} />
              </LineChart>
            </ResponsiveContainer>
          </div>
        </div>
      </div>
      
      <div className="bg-gray-50 p-4 rounded-lg shadow">
        <h2 className="text-xl font-semibold mb-4">Comprehensive Data Table</h2>
        <div className="overflow-x-auto">
          <table className="min-w-full bg-white">
            <thead>
              <tr className="bg-gray-200 text-gray-600 uppercase text-sm leading-normal">
                <th className="py-3 px-6 text-left">Dataset</th>
                <th className="py-3 px-6 text-center" colSpan="3">Edge</th>
                <th className="py-3 px-6 text-center" colSpan="2">Triangle</th>
                <th className="py-3 px-6 text-center" colSpan="2">4-Clique</th>
                <th className="py-3 px-6 text-center" colSpan="2">5-Clique</th>
                <th className="py-3 px-6 text-center" colSpan="2">6-Clique</th>
                <th className="py-3 px-6 text-center" colSpan="2">2-Star</th>
                <th className="py-3 px-6 text-center" colSpan="2">Diamond</th>
              </tr>
              <tr className="bg-gray-100 text-gray-600 text-sm leading-normal">
                <th className="py-3 px-6 text-left"></th>
                <th className="py-3 px-2 text-center">ρ-total</th>
                <th className="py-3 px-2 text-center">ρ-opt</th>
                <th className="py-3 px-2 text-center">ρ(EDS,Ψ)</th>
                <th className="py-3 px-2 text-center">ρ-opt</th>
                <th className="py-3 px-2 text-center">ρ(EDS,Ψ)</th>
                <th className="py-3 px-2 text-center">ρ-opt</th>
                <th className="py-3 px-2 text-center">ρ(EDS,Ψ)</th>
                <th className="py-3 px-2 text-center">ρ-opt</th>
                <th className="py-3 px-2 text-center">ρ(EDS,Ψ)</th>
                <th className="py-3 px-2 text-center">ρ-opt</th>
                <th className="py-3 px-2 text-center">ρ(EDS,Ψ)</th>
                <th className="py-3 px-2 text-center">ρ-opt</th>
                <th className="py-3 px-2 text-center">ρ(EDS,Ψ)</th>
                <th className="py-3 px-2 text-center">ρ-opt</th>
                <th className="py-3 px-2 text-center">ρ(EDS,Ψ)</th>
              </tr>
            </thead>
            <tbody className="text-gray-600 text-sm">
              <tr className="border-b border-gray-200 hover:bg-gray-100">
                <td className="py-3 px-6 text-left">S-DBLP</td>
                <td className="py-3 px-2 text-center">6</td>
                <td className="py-3 px-2 text-center">22</td>
                <td className="py-3 px-2 text-center">22</td>
                <td className="py-3 px-2 text-center">55</td>
                <td className="py-3 px-2 text-center">55</td>
                <td className="py-3 px-2 text-center">99</td>
                <td className="py-3 px-2 text-center">99</td>
                <td className="py-3 px-2 text-center">132</td>
                <td className="py-3 px-2 text-center">132</td>
                <td className="py-3 px-2 text-center">73.5</td>
                <td className="py-3 px-2 text-center">66</td>
                <td className="py-3 px-2 text-center">165</td>
                <td className="py-3 px-2 text-center">165</td>
                <td className="py-3 px-2 text-center">165</td>
                <td className="py-3 px-2 text-center">165</td>
              </tr>
              <tr className="border-b border-gray-200 hover:bg-gray-100">
                <td className="py-3 px-6 text-left">Yeast</td>
                <td className="py-3 px-2 text-center">3.13</td>
                <td className="py-3 px-2 text-center">2.11</td>
                <td className="py-3 px-2 text-center">0.467</td>
                <td className="py-3 px-2 text-center">0.67</td>
                <td className="py-3 px-2 text-center">0.0</td>
                <td className="py-3 px-2 text-center">0.0</td>
                <td className="py-3 px-2 text-center">0.0</td>
                <td className="py-3 px-2 text-center">0.0</td>
                <td className="py-3 px-2 text-center">0.0</td>
                <td className="py-3 px-2 text-center">111.3</td>
                <td className="py-3 px-2 text-center">18.13</td>
                <td className="py-3 px-2 text-center">20</td>
                <td className="py-3 px-2 text-center">19.2</td>
                <td className="py-3 px-2 text-center">20</td>
                <td className="py-3 px-2 text-center">19.2</td>
              </tr>
              <tr className="border-b border-gray-200 hover:bg-gray-100">
                <td className="py-3 px-6 text-left">Netscience</td>
                <td className="py-3 px-2 text-center">9.50</td>
                <td className="py-3 px-2 text-center">57.25</td>
                <td className="py-3 px-2 text-center">57.25</td>
                <td className="py-3 px-2 text-center">242.3</td>
                <td className="py-3 px-2 text-center">242.3</td>
                <td className="py-3 px-2 text-center">775.2</td>
                <td className="py-3 px-2 text-center">775.2</td>
                <td className="py-3 px-2 text-center">1938</td>
                <td className="py-3 px-2 text-center">1938</td>
                <td className="py-3 px-2 text-center">171</td>
                <td className="py-3 px-2 text-center">171</td>
                <td className="py-3 px-2 text-center">726.8</td>
                <td className="py-3 px-2 text-center">726.8</td>
                <td className="py-3 px-2 text-center">726.8</td>
                <td className="py-3 px-2 text-center">726.8</td>
              </tr>
              <tr className="border-b border-gray-200 hover:bg-gray-100">
                <td className="py-3 px-6 text-left">As-733</td>
                <td className="py-3 px-2 text-center">8.19</td>
                <td className="py-3 px-2 text-center">31.43</td>
                <td className="py-3 px-2 text-center">31.35</td>
                <td className="py-3 px-2 text-center">68.67</td>
                <td className="py-3 px-2 text-center">67.94</td>
                <td className="py-3 px-2 text-center">92.78</td>
                <td className="py-3 px-2 text-center">90.23</td>
                <td className="py-3 px-2 text-center">79.37</td>
                <td className="py-3 px-2 text-center">75.13</td>
                <td className="py-3 px-2 text-center">826.3</td>
                <td className="py-3 px-2 text-center">153.8</td>
                <td className="py-3 px-2 text-center">3376</td>
                <td className="py-3 px-2 text-center">437.7</td>
                <td className="py-3 px-2 text-center">3376</td>
                <td className="py-3 px-2 text-center">437.7</td>
              </tr>
            </tbody>
          </table>
        </div>
      </div>
    </div>
  );
};

export default NetworkDensityVisualizer;