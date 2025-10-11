# Commit staged changes with provided message
param(
    [Parameter(Mandatory=$true)]
    [string]$Message
)

git commit -m "$Message"
