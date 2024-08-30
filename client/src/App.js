
import './App.css';
import io from "socket.io-client";
import { useState } from "react";
import './font-awesome-4.7.0/css/font-awesome.css'
import logo from './logo.png'
import paper from './paper.pdf'
const socket = io.connect("http://172.29.32.186:3001");

function App() {

  
  const [tp,setTp]=useState(<i className="fa fa-spinner fa-pulse fa-fw"></i>);
  const [th,setTh]=useState( <i className="fa fa-spinner fa-pulse fa-fw"></i>);
  const [bpm,setBPM]=useState( <i className="fa fa-spinner fa-pulse fa-fw"></i>);

  socket.on('serialdata', function (dataSerial) {
    
    const x = dataSerial;
  console.log(x);
    if(x.charAt(0)==="P"){
      
      setTp(x.split("P").join('')+"°C");
      console.log("Temperatura 1:",tp);
  
    }
    else if(x.charAt(0)==="H"){
      
      setTh(x.split("H").join('')+"°C");
      console.log("Temperatura 2:",th);
    }
    else if(x.charAt(0)==="B"){
      
      setBPM(x.split("B").join('')+" BPM");
      console.log("BPM:",bpm);
    }
  });


  return (
    <div className="App">
      <nav>
      <a href={paper} download='Documento'>
         <img src={logo} alt="description" width="200" height="96" />
         </a>
   

      </nav>
      <main>
      <div id="temp">
      <div id="divTP">
      <h3 id="subtitulo">Temperatura del paciente</h3>
        <h2>{tp}</h2>
      </div>
      <div id="divTH">
      <h3 id="subtitulo">Temperatura de la habitación</h3>
      <h2>{th}</h2>
      </div>
      </div>
      <div id="divPulso">
        <h3 id="subtitulo">Pulso</h3>
        <h2>{bpm}</h2>
      </div>

      </main>
      <footer id="footer">
      <a id="socials"  href="https://www.instagram.com/expotec.utp/" target="_blank"  rel="noreferrer">
          <i id ="icon"className="fa fa-instagram" aria-hidden="true"></i>
          <div id="tag">@expotec.utp</div>
        </a>
      </footer>
    </div>
  );
}

export default App;
