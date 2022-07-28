function processInput() {
    console.log("Submit pressed.");
    if (document.getElementById("contactInput").value) {
        console.log("Content valid.")
        document.getElementById("contactStatus").innerHTML = "Thank you for your input. We will be in contact shortly.";
    }
    else {
        console.log("Content not valid.")
        document.getElementById("contactStatus").innerHTML = "Please write your message into the provided input field.";
    }

}