<body>

<div class="container">

<h1>Arduino Self-Debugging Embedded System</h1>

<p>
<strong>Project Name:</strong> Arduino Self-Debugging Embedded System<br>
<strong>Category:</strong> Embedded Systems / Fault Tolerant Firmware<br>
<strong>Platform:</strong> Arduino Uno / Arduino Nano / ATmega328P
</p>

<hr>

<h2>Project Overview</h2>

<p>
This project implements a self-diagnosing and self-recovering embedded operating
system for Arduino microcontrollers.
</p>

<p>
Unlike traditional Arduino applications that hang or behave erratically when a
fault occurs, this system is designed to detect failures, preserve diagnostic
information, and restore operation automatically.
</p>

<hr>

<h2>System Capabilities</h2>

<p>The Program is designed to:</p>

<ul>
    <li>Detect runtime failures</li>
    <li>Identify the root cause of the failure</li>
    <li>Persistently log crash information</li>
    <li>Recover from the failure automatically</li>
    <li>Provide detailed diagnostic information after reboot</li>
</ul>

<hr>

<h2>Features</h2>

<ul>
    <li>Hardware Watchdog Timer protection</li>
    <li>Reset reason detection (MCUSR register)</li>
    <li>EEPROM persistent crash logging</li>
    <li>Sensor value snapshot capture</li>
    <li>Battery voltage monitoring</li>
    <li>Loop execution timing analysis</li>
    <li>Automatic fault recovery</li>
    <li>Boot-time diagnostic reporting</li>
    <li>RTOS-style task scheduler</li>
</ul>

<hr>

<h2>Hardware Requirements</h2>

<h3>Required</h3>
<ul>
    <li>Arduino Uno or Arduino Nano</li>
    <li>USB cable</li>
</ul>

<h3>Optional</h3>
<ul>
    <li>Voltage divider (for battery monitoring)</li>
    <li>LED display (I2C)</li>
    <li>Buzzer</li>
    <li>Motor driver</li>
    <li>SD card module</li>
</ul>

<hr>

<h2>Error Code Reference</h2>

<table>
    <tr>
        <th>Code</th>
        <th>Description</th>
    </tr>
    <tr><td>0</td><td>No error</td></tr>
    <tr><td>1</td><td>Watchdog freeze</td></tr>
    <tr><td>2</td><td>Low voltage</td></tr>
    <tr><td>3</td><td>Sensor timeout</td></tr>
    <tr><td>4</td><td>Invalid sensor data</td></tr>
    <tr><td>5</td><td>Memory fault</td></tr>
    <tr><td>6</td><td>EEPROM failure</td></tr>
    <tr><td>7</td><td>Communication loss</td></tr>
    <tr><td>8</td><td>Loop stall</td></tr>
    <tr><td>9</td><td>Device fault</td></tr>
</table>

<hr>

<h2>Diagnostic Information Stored</h2>

<ul>
    <li>Error code</li>
    <li>Reset reason</li>
    <li>System uptime</li>
    <li>Sensor values at failure</li>
    <li>Battery voltage snapshot</li>
    <li>Loop execution time</li>
</ul>

<p>
All diagnostic data is stored in EEPROM and preserved even after power loss.
</p>

<hr>

<h2>Automatic Recovery Process</h2>

<ol>
    <li>Runtime fault detected</li>
    <li>System state captured</li>
    <li>Crash data written to EEPROM</li>
    <li>Controlled system reset initiated</li>
    <li>Diagnostic report displayed after reboot</li>
</ol>

<hr>


<hr>

<div class="footer">
    <p><strong>Author:</strong> Ashwin</p>
    <p><strong>Status:</strong> Stable</p>
</div>

</div>

</body>
</html>
