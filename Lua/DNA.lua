local n = io.stdin:read("*n", "*l")

local genes = {}

for token in string.gmatch(io.stdin:read("*l"):gsub("%s+$", ""), "[^%s]+") do
    table.insert(genes, token)
end

local health = {}

for token in string.gmatch(io.stdin:read("*l"):gsub("%s+$", ""), "[^%s]+") do
    table.insert(health, tonumber(token))
end

local s = io.stdin:read("*n", "*l")

local MinimumHealth = math.huge
local MaximumHealth = 0

function RecursiveGeneFind(String, Gene, FoundIndices, Init)
    local Start, End = string.find(String, Gene, Init)
    
    if Start then
        FoundIndices[Start] = true
        
        if Start < #String then
            RecursiveGeneFind(String, Gene, FoundIndices, Start + 1)
        end
    end
end

for sitr = 1, s do
    local firstmultipleinput = {}
    for token in string.gmatch(io.stdin:read("*l"):gsub("%s+$", ""), "[^%s]+") do
        table.insert(firstmultipleinput, token)
    end

    local first = 1 + tonumber(firstmultipleinput[1]:match("^%s*(.-)%s*$"))

    local last = 1 + tonumber(firstmultipleinput[2]:match("^%s*(.-)%s*$"))

    local d = firstmultipleinput[3]
    
    local DNAHealth = 0
    
    for GeneIndex = first, last do
        local Gene = genes[GeneIndex]
        
        local FoundIndices = {}
        
        RecursiveGeneFind(d, Gene, FoundIndices, 1)
        
        for Index, Value in pairs(FoundIndices) do
            DNAHealth = DNAHealth + health[GeneIndex]
        end
    end
    
    if DNAHealth > MaximumHealth then
        MaximumHealth = DNAHealth
    elseif DNAHealth < MinimumHealth then
        MinimumHealth = DNAHealth
    end
end

print(MinimumHealth .. " " .. MaximumHealth)


